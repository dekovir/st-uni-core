#pragma once
#include "unicore/Defs.hpp"

namespace unicore::Utility
{
	class ExecuteActionInCtor
	{
	public:
		explicit ExecuteActionInCtor(const Action<>& action);
	};

#define UNICORE_CONCAT_(a, b) a ## b
#define UNICORE_CONCAT(a, b) UNICORE_CONCAT_(a, b)

#define UNICORE_EXECUTE_ACTION_NAME(name, action_code) \
	static const Utility::ExecuteActionInCtor name(action_code)

#define UNICORE_EXECUTE_ACTION(action_code) \
	UNICORE_EXECUTE_ACTION_NAME(UNICORE_CONCAT(s_execute_action_, __LINE__), action_code)

#define UNICORE_EXECUTE_CODE_BLOCK(name) \
	static void UNICORE_CONCAT(name, _auto_register_function) (); \
	namespace { \
		struct UNICORE_CONCAT(name, _auto_execute) { \
			UNICORE_CONCAT(name, _auto_execute) () { \
				UNICORE_CONCAT(name, _auto_register_function) (); \
			} \
		}; \
	} \
	static const UNICORE_CONCAT(name, _auto_execute) UNICORE_CONCAT(UNICORE_CONCAT(s_auto_execute_, name), __LINE__); \
	void UNICORE_CONCAT(name, _auto_register_function)()

	template<typename T>
	class DataView
	{
	public:
		constexpr DataView()
			: _data(nullptr), _size(0) {}

		constexpr DataView(T* data, size_t size) noexcept
			: _data(data), _size(size) {}

		UC_NODISCARD constexpr size_t size() const { return _size; }
		UC_NODISCARD constexpr size_t size_bytes() const { return _size * sizeof(T); }
		UC_NODISCARD constexpr bool empty() const { return _size == 0; }

		UC_NODISCARD constexpr T* data() const { return _data; }

	protected:
		T* _data;
		size_t _size;
	};
}