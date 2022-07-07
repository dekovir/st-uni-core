#include "unicore/wasm/WAModule.hpp"

#include <m3_env.h>

#include "unicore/Logger.hpp"
#include "unicore/wasm/WARuntime.hpp"

namespace unicore
{
	WAModule::WAModule(IM3Module handle, const Shared<MemoryChunk>& memory, Logger& logger)
		: WAObject(handle), _memory(memory), _logger(logger)
	{
	}

	WAModule::~WAModule()
	{
		if (_runtime == nullptr)
			m3_FreeModule(_handle);
	}

	void WAModule::set_name(StringView name)
	{
		m3_SetModuleName(_handle, name.data());
	}

	bool WAModule::link_function_raw(StringView module_name,
		StringView function_name, StringView signature, M3RawCall func)
	{
		const auto error = m3_LinkRawFunction(_handle,
			module_name.data(), function_name.data(), signature.data(), func);
		if (!error) return true;

		UC_LOG_ERROR(_logger) << error;
		return false;
	}

	WAGlobal WAModule::find_global(StringView name) const
	{
		return WAGlobal(m3_FindGlobal(_handle, name.data()));
	}

	void WAModule::enum_globals(const Action<WAGlobal>& action) const
	{
		for (unsigned i = 0; i < _handle->numGlobals; i++)
			action(WAGlobal(&_handle->globals[i]));
	}

	bool WAModule::load_to(WARuntime& runtime)
	{
		if (_runtime != nullptr)
		{
			UC_LOG_ERROR(_logger) << "Module already loaded";
			return false;
		}

		const auto error = m3_LoadModule(runtime.handle(), _handle);
		if (!error)
		{
			_runtime = &runtime;
			return true;
		}

		UC_LOG_ERROR(_logger) << error;
		return false;
	}

	bool WAModule::compile()
	{
		const auto error = m3_CompileModule(_handle);
		if (!error) return true;

		UC_LOG_ERROR(_logger) << error;
		return false;
	}

	bool WAModule::run_start()
	{
		const auto error = m3_RunStart(_handle);
		if (!error) return true;

		UC_LOG_ERROR(_logger) << error;
		return false;
	}
}