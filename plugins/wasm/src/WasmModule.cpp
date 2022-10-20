#include "unicore/wasm/WasmModule.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/wasm/WasmRuntime.hpp"

namespace unicore
{
	WasmModule::WasmModule(IM3Module handle, const Shared<BinaryData>& data, Logger& logger)
		: WasmObject(handle), _data(data), _logger(logger)
	{
	}

	WasmModule::~WasmModule()
	{
		if (_runtime == nullptr)
			m3_FreeModule(_handle);
	}

	size_t WasmModule::get_system_memory_use() const
	{
		return sizeof(WasmModule) + (_data ? _data->get_system_memory_use() : 0);
	}

	StringView WasmModule::get_name() const
	{
		return m3_GetModuleName(_handle);
	}

	void WasmModule::set_name(StringView name)
	{
		m3_SetModuleName(_handle, name.data());
	}

	bool WasmModule::link_function_raw(StringView module_name,
		StringView function_name, StringView signature, M3RawCall func)
	{
		if (const auto error = m3_LinkRawFunction(_handle,
			module_name.data(), function_name.data(), signature.data(), func))
		{
			UC_LOG_ERROR(_logger) << error;
			return false;
		}

		return true;
	}

	bool WasmModule::load_to(WasmRuntime& runtime)
	{
		if (_runtime != nullptr)
		{
			UC_LOG_ERROR(_logger) << "Module already loaded";
			return false;
		}

		if (const auto error = m3_LoadModule(runtime.handle(), _handle))
		{
			UC_LOG_ERROR(_logger) << error;
			return false;
		}

		_runtime = &runtime;
		return true;
	}

	bool WasmModule::compile()
	{
		if (const auto error = m3_CompileModule(_handle))
		{
			UC_LOG_ERROR(_logger) << error;
			return false;
		}

		return true;
	}

	bool WasmModule::run_start()
	{
		if (const auto error = m3_RunStart(_handle))
		{
			UC_LOG_ERROR(_logger) << error;
			return false;
		}

		return true;
	}
}