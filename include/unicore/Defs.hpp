#pragma once
#include <cstdint>
#include <type_traits>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <optional>
#include <memory>
#include <functional>
#include <cassert>
#include <typeindex>
#include <algorithm>

#if defined (_WIN32)

#	define UNICORE_PLATFORM_WINDOWS
#	define UNICORE_PLATFORM_DESKTOP

#elif defined(__APPLE__)

#	include <TargetConditionals.h>
#	define UNICORE_PLATFORM_APPLE
#	if TARGET_OS_IPHONE
#		define UNICORE_PLATFORM_IOS
#		define UNICORE_PLATFORM_MOBILE

#		if TARGET_IPHONE_SIMULATOR
#			define UNICORE_PLATFORM_IOS_SIMULATOR
#		endif
#	else
#		define UNICORE_PLATFORM_OSX
#		define UNICORE_PLATFORM_DESKTOP
#	endif

#elif defined(__ANDROID__)

#	define UNICORE_PLATFORM_ANDROID
#	define UNICORE_PLATFORM_MOBILE

#elif defined(EMSCRIPTEN)

#	define UNICORE_PLATFORM_EMSCRIPTEN
#	define UNICORE_PLATFORM_WEB
#	include <emscripten/emscripten.h>

#else

static_assert(false, "Unknown platform");

#endif

#if defined(_DEBUG) || defined(DEBUG)
#	define UNICORE_DEBUG
#else
#	define UNICORE_RELEASE
#endif

namespace unicore
{
	template<typename T, size_t N>
	using Array = std::array<T, N>;

	template<typename T>
	using List = std::vector<T>;

	template<typename T>
	using Set = std::set<T>;

	template<typename TKey, typename TValue>
	using Dictionary = std::map<TKey, TValue>;

	template<typename T>
	using BasicString = std::basic_string<T>;
	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;

	template<typename T>
	using BasicStringView = std::basic_string_view<T>;
	using StringView = BasicStringView<char>;
	using WStringView = BasicStringView<wchar_t>;

	template<typename T>
	using Optional = std::optional<T>;

	template<typename T>
	using HashFunc = std::hash<T>;

	template <class This, class... Rest>
	using Tuple = std::tuple<This, Rest...>;

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, class... Types>
	static inline Shared<T> make_shared(Types&&... args)
	{
		return std::make_shared<T>(args...);
	}

	using TypeInfo = std::type_info;

	template<typename T>
	static inline const TypeInfo& get_type_info()
	{
		return typeid(T);
	}

	using TypeIndex = std::type_index;
	template<typename T>
	static inline TypeIndex get_type_index()
	{
		return TypeIndex(get_type_info<T>());
	}
}

#define UC_NODISCARD [[nodiscard]]

#define UC_ASSERT(cond) assert(cond)
#define UC_ASSERT_MSG(cond, msg) assert(cond && msg)

#define UC_ASSERT_ALWAYS() assert(false)
#define UC_ASSERT_ALWAYS_MSG(msg) assert(false && msg)

#define UC_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#define UC_STATIC_ASSERT_ALWAYS(msg) static_assert(false, msg)