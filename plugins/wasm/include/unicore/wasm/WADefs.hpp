#pragma once
#include "unicore/Defs.hpp"
#include <wasm3.h>

namespace unicore
{
	template<typename T>
	class WAValue
	{
	public:
		explicit WAValue(T handle)
			: _handle(handle) {}

		T handle() const { return _handle; }

	protected:
		T _handle;
	};

	template<typename T>
	class WAObject : public WAValue<T>
	{
	public:
		explicit WAObject(T handle)
			: WAValue<T>(handle)
		{
		}
	};
}