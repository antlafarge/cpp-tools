#pragma once

#include <stdexcept>
#include <type_traits>

#if ((defined(_HAS_CXX17) && !_HAS_CXX17) || (!defined(_HAS_CXX17) && __cplusplus < 201703L))

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

namespace std
{
	struct nullopt_t
	{
		explicit constexpr nullopt_t() = default;
	};

	constexpr nullopt_t nullopt;

	struct in_place_t
	{
		explicit in_place_t() = default;
	};

	constexpr in_place_t in_place;

	template<class T>
	class optional
	{
	public:

		constexpr optional() noexcept = default;

		constexpr optional(std::nullopt_t) noexcept
			: _hasValue(false)
			, _value()
		{
		}

		constexpr optional(const optional& other)
			: _hasValue(other._hasValue)
			, _value(other._value)
		{
		}

		constexpr optional(const optional&& other) noexcept
			: _hasValue(std::move(other._hasValue))
			, _value(std::move(other._value))
		{
		}

		template<class U>
		constexpr optional(const optional<U>& other)
			: _hasValue(other._hasValue)
			, _value(other._value)
		{
		}

		template<class U>
		constexpr optional(optional<U>&& other)
			: _hasValue(std::move(other._hasValue))
			, _value(std::move(other._value))
		{
		}

		template<class... Args>
		constexpr explicit optional(in_place_t, Args&&... args)
			: _hasValue(true)
			, _value(std::forward<Args>(args)...)
		{
		}

		template<class U, class... Args>
		constexpr explicit optional(typename std::enable_if<std::is_constructible<T, std::initializer_list<U>&, Args&&...>::value, in_place_t>::type in_place, std::initializer_list<U> ilist, Args&&... args)
			: _hasValue(true)
			, _value(ilist, std::forward<Args>(args)...)
		{
		}

		template<class U = T>
		constexpr optional(U&& value)
			: _hasValue(true)
			, _value(value)
		{
		}

		~optional() = default;

		constexpr optional& operator=(std::nullopt_t) noexcept
		{
			_hasValue = false;
			_value = T();
			return *this;
		}

		constexpr optional& operator=(const optional& other)
		{
			_hasValue = other._hasValue;
			_value = other._value;
			return *this;
		}

		constexpr optional& operator=(optional&& other) noexcept
		{
			_hasValue = std::move(other._hasValue);
			_value = std::move(other._value);
			return *this;
		}

		template<class U = T>
		constexpr optional& operator=(U&& value)
		{
			_hasValue = true;
			_value = value;
			return *this;
		}

		template<class U>
		constexpr optional& operator=(const optional<U>& other)
		{
			_hasValue = other._hasValue;
			_value = other._value;
			return *this;
		}

		template<class U>
		constexpr optional& operator=(optional<U>&& other)
		{
			_hasValue = std::move(other._hasValue);
			_value = std::move(other._value);
			return *this;
		}

		constexpr const T* operator->() const noexcept
		{
			return &_value;
		}

		constexpr T* operator->() noexcept
		{
			return &_value;
		}

		constexpr const T& operator*() const& noexcept
		{
			return _value;
		}

		constexpr T& operator*() & noexcept
		{
			return _value;
		}

		constexpr const T&& operator*() const&& noexcept
		{
			return std::move(_value);
		}

		constexpr T&& operator*() && noexcept
		{
			return std::move(_value);
		}

		constexpr explicit operator bool() const noexcept
		{
			return _hasValue;
		}

		constexpr bool has_value() const noexcept
		{
			return _hasValue;
		}

		constexpr T& value()&
		{
			if (_hasValue) [[likely]]
			{
				return _value;
			}
			throw std::runtime_error("bad_optional_access");
		}

		constexpr const T& value() const&
		{
			if (_hasValue) [[likely]]
			{
				return _value;
			}
			throw std::runtime_error("bad_optional_access");
		}

		constexpr T&& value()&&
		{
			if (_hasValue) [[likely]]
			{
				return std::move(_value);
			}
			throw std::runtime_error("bad_optional_access");
		}

		constexpr const T&& value() const&&
		{
			if (_hasValue) [[likely]]
			{
				return std::move(_value);
			}
			throw std::runtime_error("bad_optional_access");
		}

		template<class U>
		constexpr T value_or(U&& default_value) const&
		{
			return bool(*this) ? **this : static_cast<T>(std::forward<U>(default_value));
		}

		template< class U >
		constexpr T value_or(U&& default_value)&&
		{
			return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(default_value));
		}

		constexpr void swap(optional& other) noexcept
		{
			std::swap(_hasValue, other._hasValue);
			std::swap(_value, other._value);
		}

		constexpr void reset() noexcept
		{
			_hasValue = false;
			_value = T();
		}

		template<class... Args>
		constexpr T& emplace(Args&&... args)
		{
			_hasValue = true;
			_value = T(std::forward<Args>(args)...);
		}

		template<class U, class... Args>
		constexpr typename std::enable_if<std::is_constructible<T, std::initializer_list<U>&, Args&&...>::value, T&>::type emplace(std::initializer_list<U> ilist, Args&&... args)
		{
			_hasValue = true;
			_value = T(ilist, std::forward<Args>(args)...);
		}

	private:

		bool _hasValue = false;
		T _value;
	};

	template<class T, class U>
	constexpr bool operator==(const optional<T>& lhs, const optional<U>& rhs)
	{
		bool lhv = lhs.has_value(), rhv = rhs.has_value();
		return (lhv == rhv) && (!lhv || !rhv || lhs.value() == rhs.value());
	}

	template<class T, class U>
	constexpr bool operator!=(const optional<T>& lhs, const optional<U>& rhs)
	{
		return (lhs.has_value() != rhs.has_value()) || (lhs.has_value() && (lhs.value() != rhs.value()));
	}

	template<class T, class U>
	constexpr bool operator<(const optional<T>& lhs, const optional<U>& rhs)
	{
		return rhs.has_value() && (!lhs.has_value() || lhs.value() < rhs.value());
	}

	template<class T, class U>
	constexpr bool operator<=(const optional<T>& lhs, const optional<U>& rhs)
	{
		return rhs.has_value() && (!lhs.has_value() || lhs.value() <= rhs.value());
	}

	template<class T, class U>
	constexpr bool operator>(const optional<T>& lhs, const optional<U>& rhs)
	{
		return lhs.has_value() && (!rhs.has_value() || lhs.value() > rhs.value());
	}

	template<class T, class U>
	constexpr bool operator>=(const optional<T>& lhs, const optional<U>& rhs)
	{
		return lhs.has_value() && (!rhs.has_value() || lhs.value() >= rhs.value());
	}

	template<class T>
	constexpr bool operator==(const optional<T>& opt, nullopt_t) noexcept
	{
		return !opt._hasValue;
	}

	template<class T, class U>
	constexpr bool operator==(const optional<T>& opt, const U& value)
	{
		return opt._hasValue && opt._value == value;
	}

	template<class T, class U>
	constexpr bool operator==(const T& value, const optional<U>& opt)
	{
		return opt._hasValue && opt._value == value;
	}

	template<class T, class U>
	constexpr bool operator!=(const optional<T>& opt, const U& value)
	{
		return !opt._hasValue || opt._value != value;
	}

	template<class T, class U>
	constexpr bool operator!=(const T& value, const optional<U>& opt)
	{
		return !opt._hasValue || opt._value != value;
	}

	template<class T, class U>
	constexpr bool operator<(const optional<T>& opt, const U& value)
	{
		return !opt._hasValue || opt._value < value;
	}

	template<class T, class U>
	constexpr bool operator<(const T& value, const optional<U>& opt)
	{
		return opt._hasValue && value < opt._value;
	}

	template<class T, class U>
	constexpr bool operator<=(const optional<T>& opt, const U& value)
	{
		return !opt._hasValue || opt._value <= value;
	}

	template<class T, class U>
	constexpr bool operator<=(const T& value, const optional<U>& opt)
	{
		return opt._hasValue && value <= opt._value;
	}

	template<class T, class U>
	constexpr bool operator>(const optional<T>& opt, const U& value)
	{
		return opt._hasValue && opt._value > value;
	}

	template<class T, class U>
	constexpr bool operator>(const T& value, const optional<U>& opt)
	{
		return opt._hasValue && value > opt._value;
	}

	template<class T, class U>
	constexpr bool operator>=(const optional<T>& opt, const U& value)
	{
		return opt._hasValue && opt._value >= value;
	}

	template<class T, class U>
	constexpr bool operator>=(const T& value, const optional<U>& opt)
	{
		return opt._hasValue && value >= opt._value;
	}

	template<class T>
	constexpr std::optional<std::decay_t<T>> make_optional(T&& value)
	{
		return std::optional<std::decay_t<T>>(std::forward<T>(value));
	}

	template<class T, class... Args>
	constexpr std::optional<T> make_optional(Args&&... args)
	{
		return std::optional<T>(std::in_place, std::forward<Args>(args)...);
	}
	
	template<class T, class U, class... Args>
	constexpr std::optional<T> make_optional(std::initializer_list<U> il, Args&&... args)
	{
		return std::optional<T>(std::in_place, il, std::forward<Args>(args)...);
	}
}

#pragma GCC diagnostic pop

#else

#include <optional>

#endif
