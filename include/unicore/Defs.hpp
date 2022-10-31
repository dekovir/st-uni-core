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
#include <numeric>

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

	using Bool = bool;

	using Char = char;
	using CharW = wchar_t;
	using Char16 = char16_t;
	using Char32 = char32_t;

	using Int = int;
	using UInt = unsigned int;
	using IntPtr = intptr_t;

	using Int8 = int8_t;
	using Int16 = int16_t;
	using Int32 = int32_t;
	using Int64 = int64_t;

	using UInt8 = uint8_t;
	using UInt16 = uint16_t;
	using UInt32 = uint32_t;
	using UInt64 = uint64_t;

	using Float = float;
	using Double = double;

	using Byte = UInt8;
	using Size = size_t;

	namespace arithmetic
	{
		template<typename T>
		using Plus = std::plus<T>;

		template<typename T>
		using Minus = std::minus<T>;

		template<typename T>
		using Multiply = std::multiplies<T>;

		template<typename T>
		using Divide = std::divides<T>;
	}

	namespace comparison
	{
		template<typename T>
		using Equal = std::equal_to<T>;

		template<typename T>
		using NotEqual = std::not_equal_to<T>;

		template<typename T>
		using Greater = std::greater<T>;

		template<typename T>
		using Less = std::less<T>;

		template<typename T>
		using GreaterEqual = std::greater_equal<T>;

		template<typename T>
		using LessEqual = std::less_equal<T>;
	}

	template<size_t N>
	using Bitset = std::bitset<N>;

	template<typename T, size_t N>
	using Array = std::array<T, N>;

	template<typename T>
	using List = std::vector<T>;

	template<typename T, class Sort = comparison::Less<T>>
	using Set = std::set<T, Sort>;

	template<typename TKey, typename TValue, class Sort = comparison::Less<TKey>>
	using Dictionary = std::map<TKey, TValue, Sort>;

	template<typename TKey, typename TValue, class Sort = comparison::Less<TKey>>
	using DictionaryMulti = std::multimap<TKey, TValue, Sort>;

	template<typename T>
	using BasicString = std::basic_string<T>;
	using String = BasicString<Char>;
	using StringW = BasicString<CharW>;
	using String16 = BasicString<Char16>;
	using String32 = BasicString<Char32>;

	template<typename T>
	using BasicStringView = std::basic_string_view<T>;
	using StringView = BasicStringView<Char>;
	using StringViewW = BasicStringView<CharW>;
	using StringView16 = BasicStringView<Char16>;
	using StringView32 = BasicStringView<Char32>;

	template<typename T>
	using Optional = std::optional<T>;

	using Any = std::any;

	template <typename This, typename... Rest>
	using StdVariant = std::variant<This, Rest...>;

	template<typename T>
	using HashFunc = std::hash<T>;

	template <typename This, typename... Rest>
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

	template<typename... Types>
	extern List<TypeIndex> get_type_indices() { return { get_type_index<Types>()... }; }

	template<typename T>
	using Ref = std::reference_wrapper<T>;

	template<typename T>
	using Weak = std::weak_ptr<T>;

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, typename... Types>
	static inline Shared<T> make_shared(Types&&... args)
	{
		return std::make_shared<T>(std::forward<Types>(args)...);
	}

	template<typename T, typename... Types>
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

#define UC_TYPE_DEFAULT_MOVE(Type) \
	Type(Type&&) noexcept = default; \
	Type& operator= (Type&&) noexcept = default

#define UC_TYPE_DELETE_MOVE(Type) \
	Type(Type&&) = delete; \
	Type& operator= (Type&&) = delete

#define UC_TYPE_DEFAULT_COPY(Type) \
	Type(Type const&) noexcept = default; \
	Type& operator= (Type const&) noexcept = default

#define UC_TYPE_DELETE_COPY(Type) \
	Type(Type const&) = delete; \
	Type& operator= (Type const&) = delete

#define UC_TYPE_DEFAULT_MOVE_COPY(Type) \
	UC_TYPE_DEFAULT_MOVE(Type); \
	UC_TYPE_DEFAULT_COPY(Type)

#define UC_TYPE_DELETE_MOVE_COPY(Type) \
	UC_TYPE_DELETE_MOVE(Type); \
	UC_TYPE_DELETE_COPY(Type)

#define UC_TYPE_DEFAULT_DTOR_MOVE_COPY(Type) \
	~Type() = default; \
	UC_TYPE_DEFAULT_MOVE_COPY(Type)

#define UC_TYPE_DELETE_DTOR_MOVE_COPY(Type) \
	~Type() = delete; \
	UC_TYPE_DELETE_MOVE_COPY(Type)

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