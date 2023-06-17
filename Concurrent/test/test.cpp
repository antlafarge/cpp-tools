#include "Concurrent.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

const int threadMax = 8;
const int writeMax = 1000;

std::atomic_int threadsCountWaiting;
std::atomic_int threadsCountFinished;

ConcurrentResource<uint64_t> concurrentResource(0, ConcurrentResourcePriority::Queue);

std::vector<std::thread> threadsRead;
std::vector<std::thread> threadsWrite;

template<typename T>
inline void log(const T& t)
{
	static std::recursive_mutex mutex;
	std::lock_guard<std::recursive_mutex> lg(mutex);
	std::cout << t << std::endl;
}

int main()
{
	std::this_thread::sleep_for(2000ms);

	threadsCountWaiting = 0;
	threadsCountFinished = 0;

	log("Started");
	
	auto t1 = std::chrono::steady_clock::now();

	{
		auto writableResource = concurrentResource.writeLock();

		for (int i = 0; i < threadMax; i++)
		{
			threadsRead.push_back(std::thread([i]()
			{
				auto id = "R" + std::to_string(i + 1);
				
				log(id + " is waiting");
				
				uint64_t readCount = 0;

				threadsCountWaiting++;

				// We stop to read when all write threads finished
				while (threadsCountFinished != threadMax)
				{
					{
						auto readableResource = concurrentResource.readLock();
						auto a = *readableResource;
						(void)a;
					}

					readCount++;
				}

				log(id + " finished (" + std::to_string(readCount) + " reads)");
			}));

			threadsWrite.push_back(std::thread([i]()
			{
				auto id = "W" + std::to_string(i + 1);

				log(id + " is waiting");

				uint64_t writeCount = 0;
				
				threadsCountWaiting++;

				for (int w = 0; w < writeMax; w++)
				{
					{
						auto writableResource = concurrentResource.writeLock();
						++(*writableResource);
					}

					writeCount++;
				}
				
				log(id + " finished (" + std::to_string(writeCount) + " writes)");

				threadsCountFinished++;
			}));
		}

		while (threadsCountWaiting != 2 * threadMax)
		{
			std::this_thread::sleep_for(10ms);
		}
		
		log("Go");

		t1 = std::chrono::steady_clock::now();
	}

	for (auto& thread : threadsWrite)
	{
		thread.join();
	}

	for (auto& thread : threadsRead)
	{
		thread.join();
	}

	auto t2 = std::chrono::steady_clock::now();

	auto elapsedTime = t2 - t1;

	auto writeCount = *concurrentResource.readLock();

	log("Finished (" + std::to_string((double)elapsedTime.count() / 1000 / 1000 / 1000) + " seconds; " + std::to_string(writeCount) + " writes)");

	return 0;
}
