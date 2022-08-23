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
}