#include "unicore/system/Utility.hpp"

namespace unicore::Utility
{
	ExecuteActionInCtor::ExecuteActionInCtor(const Action<>& action)
	{
		action();
	}
}