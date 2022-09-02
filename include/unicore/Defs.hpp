#pragma once
#include <cstdint>
#include <type_traits>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <any>
#include <bitset>
#include <string>
#include <string_view>
#include <optional>
#include <variant>
#include <memory>
#include <functional>
#include <cassert>
#include <cstring>
#include <typeindex>
#include <algorithm>
#include <utility>

#if defined (_WIN32)

#	define UNICORE_PLATFORM_WINDOWS

#elif defined(__APPLE__)

#	include <TargetConditionals.h>
#	define UNICORE_PLATFORM_APPLE
#	define UNICORE_PLATFORM_LINUX

#	if TARGET_OS_IPHONE
#		define UNICORE_PLATFORM_IOS

#		if TARGET_IPHONE_SIMULATOR
#			define UNICORE_PLATFORM_IOS_SIMULATOR
#		endif
#	else
#		define UNICORE_PLATFORM_OSX
#	endif

#elif defined(__ANDROID__)

#	define UNICORE_PLATFORM_ANDROID
#	define UNICORE_PLATFORM_LINUX

#elif defined(EMSCRIPTEN)

#	define UNICORE_PLATFORM_EMSCRIPTEN
#	define UNICORE_PLATFORM_WEB
#	define UNICORE_PLATFORM_LINUX
#	define UNICORE_PLATFORM_POSIX

#	include <emscripten/emscripten.h>

#elif defined(__linux__) || defined(__unix__)

#	define UNICORE_PLATFORM_LINUX
#	define UNICORE_PLATFORM_POSIX

#else

static_assert(false, "Unknown platform");

#endif

#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
#	define UNICORE_DEBUG
#else
#	define UNICORE_RELEASE
#endif

namespace unicore
{
	using namespace std::string_literals;
	using namespace std::string_view_literals;

	template<size_t N>
	using Bitset = std::bitset<N>;

	template<typename T, size_t N>
	using Array = std::array<T, N>;

	template<typename T>
	using List = std::vector<T>;

	template<typename T, class Sort = std::less<T>>
	using Set = std::set<T, Sort>;

	template<typename TKey, typename TValue, class Sort = std::less<TKey>>
	using Dictionary = std::map<TKey, TValue, Sort>;

	template<typename TKey, typename TValue, class Sort = std::less<TKey>>
	using DictionaryMulti = std::multimap<TKey, TValue, Sort>;

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

	using Any = std::any;

	template <class... Types>
	using Variant = std::variant<Types...>;

	template<typename T>
	using HashFunc = std::hash<T>;

	template <class This, class... Rest>
	using Tuple = std::tuple<This, Rest...>;

	using TypeInfo = std::type_info;
	using TypeIndex = std::type_index;

	template<typename T>
	using CleanType = std::remove_const<std::remove_reference<std::remove_pointer<T>>>;

	template<typename T>
	extern const TypeInfo& get_type_info() { return typeid(CleanType<T>); }

	template<typename T>
	extern const TypeInfo& get_type_info(const T& value) { return typeid(value); }

	template<typename T>
	extern TypeIndex get_type_index() { return TypeIndex(get_type_info<T>()); }

	template<typename T>
	extern TypeIndex get_type_index(const T& value) { return TypeIndex(get_type_info(value)); }

	template<typename T>
	using Weak = std::weak_ptr<T>;

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, class... Types>
	static inline Shared<T> make_shared(Types&&... args)
	{
		return std::make_shared<T>(std::forward<Types>(args)...);
	}

	template<typename T, class... Types>
	static inline Unique<T> make_unique(Types&&... args)
	{
		return std::make_unique<T>(std::forward<Types>(args)...);
	}

	template<typename T>
	using Predicate = std::function<bool(T)>;

	template<typename ... Args>
	using Action = std::function<void(Args...)>;

	template<typename Ret, typename ... Args>
	using Function = std::function<Ret(Args...)>;

	class Logger;
	class StringBuilder;
}

#define UNICODE_STRING_BUILDER_FORMAT(Type) \
	StringBuilder& operator << (StringBuilder& builder, Type value)

#define UC_NODISCARD [[nodiscard]]
#define UC_UNUSED [[maybe_unused]]

#define UC_ASSERT(cond) assert(cond)
#define UC_ASSERT_MSG(cond, msg) assert(cond && msg)

#define UC_ASSERT_ALWAYS() assert(false)
#define UC_ASSERT_ALWAYS_MSG(msg) assert(false && msg)

#define UC_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#define UC_STATIC_ASSERT_ALWAYS(msg) static_assert(false, msg)