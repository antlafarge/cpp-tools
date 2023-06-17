#include <condition_variable>
#include <cstdint>
#include <mutex>

// All waiting read operations are always executed simulaneously, and write operations are always executed one by one.
enum class ConcurrentResourcePriority
{
	// Queue the operations.
	Queue = 0,

	// Altern between read and write operations.
	Altern = 1,
	
	// Execute write operations after the next resource release.
	Write = 2, 
	
	// Execute read operations after the next resource release.
	Read = 3,
};

// Tool to manage concurrent access to a resource.
// Read operations will execute together in paralell, Write operations will execute by locking exclusively the resource.
// Use readLock to get a read-only access to the resource.
// Use writeLock to get a read and write access to the resource.
template<class TResource>
class ConcurrentResource
{
public:

	class ReadLockedResource
	{
		friend class ConcurrentResource;

	public:

		ReadLockedResource(ReadLockedResource&& other)
			: _concurrentResource(other._concurrentResource)
			, _shouldRelease(other._shouldRelease)
		{
			other._shouldRelease = false;
		}

		virtual ~ReadLockedResource()
		{
			if (_shouldRelease)
			{
				_concurrentResource.unlockRead();
			}
		}

		const TResource* operator->() const
		{
			return &_concurrentResource._resource;
		}

		const TResource& operator*() const
		{
			return _concurrentResource._resource;
		}

	private:

		ReadLockedResource(ConcurrentResource& concurrentResource)
			: _concurrentResource(concurrentResource)
			, _shouldRelease(true)
		{
			_concurrentResource.lockRead();
		}
		
		ReadLockedResource() = delete;
		ReadLockedResource(const ReadLockedResource&) = delete;

		ReadLockedResource& operator=(const ReadLockedResource&) = delete;
		ReadLockedResource& operator=(ReadLockedResource&&) = delete;

		ConcurrentResource& _concurrentResource;
		bool _shouldRelease = false;
	};

	class WriteLockedResource
	{
		friend class ConcurrentResource;

	public:

		WriteLockedResource(WriteLockedResource&& other)
			: _concurrentResource(other._concurrentResource)
			, _shouldRelease(other._shouldRelease)
		{
			other._shouldRelease = false;
		}

		virtual ~WriteLockedResource()
		{
			_concurrentResource.unlockWrite();
		}

		TResource* operator->() const
		{
			return &_concurrentResource._resource;
		}

		TResource& operator*() const
		{
			return _concurrentResource._resource;
		}

	private:

		WriteLockedResource(ConcurrentResource& concurrentResource)
			: _concurrentResource(concurrentResource)
			, _shouldRelease(true)
		{
			_concurrentResource.lockWrite();
		}

		WriteLockedResource() = delete;
		WriteLockedResource(const WriteLockedResource&) = delete;

		WriteLockedResource& operator=(const WriteLockedResource&) = delete;
		WriteLockedResource& operator=(WriteLockedResource&&) = delete;

		ConcurrentResource& _concurrentResource;
		bool _shouldRelease = false;
	};

	ConcurrentResource(TResource resource, ConcurrentResourcePriority priority = ConcurrentResourcePriority::Queue)
		: _resource(resource)
		, _priority(priority)
	{
	}

	[[nodiscard]]
	ReadLockedResource readLock()
	{
		return ReadLockedResource(*this);
	}

	[[nodiscard]]
	WriteLockedResource writeLock()
	{
		return WriteLockedResource(*this);
	}

private:

	void lockRead()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		// If there is a waiting writer which will execute before this reader
		bool somethingShouldExecuteBefore = (_priority != ConcurrentResourcePriority::Read && _waitToWrite[_currentWriteId] > 0);
		// If there is a working writer
		while (_writers > 0 || somethingShouldExecuteBefore)
		{
			somethingShouldExecuteBefore = false;
			_waitToRead++;
			// We wait 
			_readCV.wait(lock);
			_waitToRead--;
		}
		if (_priority == ConcurrentResourcePriority::Queue)
		{
			// If there is no waiting writers in the current write list
			if (_waitToWrite[_currentWriteId] == 0)
			{
				// And if there are waiting writers in the other write list
				uint8_t otherWriteId = (_currentWriteId + 1) % 2;
				if (_waitToWrite[otherWriteId] > 0)
				{
					// We have writers which wait the readers to execute
					// So we swap, the accumulated writers will execute after the read operations
					_currentWriteId = otherWriteId;
				}
			}
		}
		_readers++;
		// Manual unlock to release resource as soon as possible
		lock.unlock();
	}

	void unlockRead()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_readers--;
		// If this was the last reader AND there is a waiting writer
		if (_readers == 0 && _waitToWrite[_currentWriteId] > 0)
		{
			// We wake up a writer 
			_writeCV[_currentWriteId].notify_one();
		}
		// Manual unlock to release resource as soon as possible
		lock.unlock();
	}

	void lockWrite()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		// If there is a waiting reader/writer which will execute before this writer
		bool somethingShouldExecuteBefore = (_waitToRead > 0 || _waitToWrite[_currentWriteId] > 0);
		// If there is a working reader/writer OR somethingShouldExecuteBefore
		while (_readers > 0 || _writers > 0 || somethingShouldExecuteBefore)
		{
			somethingShouldExecuteBefore = false;
			int writeId = _currentWriteId;
			if (_priority == ConcurrentResourcePriority::Queue)
			{
				// We will execute the writer after the current write operations (and after the reader operations if there are waiting readers)
				if (_waitToRead > 0)
				{
					writeId = (_currentWriteId + 1) % 2;
				}
			}
			_waitToWrite[writeId]++;
			// We wait
			_writeCV[writeId].wait(lock);
			_waitToWrite[writeId]--;
		}
		_writers++;
		// Manual unlock to release resource as soon as possible
		lock.unlock();
	}

	void unlockWrite()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_writers--;
		if (_priority == ConcurrentResourcePriority::Queue || _priority == ConcurrentResourcePriority::Write)
		{
			// If there is another waiting writer
			if (_waitToWrite[_currentWriteId] > 0)
			{
				// We wake up a waiting writer
				_writeCV[_currentWriteId].notify_one();
			}
			// If there is a waiting reader
			else if (_waitToRead > 0)
			{
				// We wake up the waiting readers
				_readCV.notify_all();
			}
		}
		else if (_priority == ConcurrentResourcePriority::Altern || _priority == ConcurrentResourcePriority::Read)
		{
			// If there is a waiting reader
			if (_waitToRead > 0)
			{
				// We wake up the waiting readers
				_readCV.notify_all();
			}
		}
		// Manual unlock to release resource as soon as possible
		lock.unlock();
	}

private:

	TResource _resource;
	std::mutex _mutex;
	std::condition_variable _readCV;
	std::condition_variable _writeCV[2];
	uint64_t _readers = 0;
	uint64_t _writers = 0;
	uint64_t _waitToRead = 0; // TODO : Replace by a bool? (should be sufficient)
	uint64_t _waitToWrite[2] = { 0 };
	uint8_t _currentWriteId = 0;
	ConcurrentResourcePriority _priority = ConcurrentResourcePriority::Queue;
};
