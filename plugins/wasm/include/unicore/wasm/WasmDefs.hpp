#pragma once
#include "unicore/Object.hpp"
#include <wasm3.h>

namespace unicore
{
	template<typename T>
	class WasmValue
	{
	public:
		explicit WasmValue(T handle)
			: _handle(handle) {}

		T handle() const { return _handle; }

	protected:
		T _handle;
	};

	template<typename T>
	class WasmObject : public WasmValue<T>, public Object
	{
	public:
		explicit WasmObject(T handle)
			: WasmValue<T>(handle)
		{
		}
	};
}