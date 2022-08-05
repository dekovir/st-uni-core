#include "unicore/wasm/WasmRuntime.hpp"

#include <m3_env.h>

#include "unicore/Logger.hpp"
#include "unicore/wasm/WasmModule.hpp"

namespace unicore
{
	WasmRuntime::WasmRuntime(IM3Runtime handle, Logger& logger)
		: WasmObject(handle), _logger(logger)
	{
	}

	WasmRuntime::~WasmRuntime()
	{
		m3_FreeRuntime(_handle);
	}

	MemoryChunk WasmRuntime::get_memory() const
	{
		uint32_t size;
		auto data = m3_GetMemory(_handle, &size, 0);
		if (data) return MemoryChunk(data, size, false);

		UC_LOG_ERROR(_logger) << "m3_GetMemory failed";
		return {};
	}

	WasmError WasmRuntime::get_error() const
	{
		WasmError error{};
		m3_GetErrorInfo(_handle, &error.info);
		return error;
	}

	Optional<WasmFunction> WasmRuntime::find_function(StringView function_name) const
	{
		IM3Function func;
		if (const auto error = m3_FindFunction(&func, _handle, function_name.data()); !error)
			return WasmFunction(func);

		return std::nullopt;
	}

	void WasmRuntime::enum_functions(const Action<WasmFunction>& action) const
	{
		for (auto mod = _handle->modules; mod != nullptr; mod = mod->next)
		{
			//const auto count = mod->numFunctions + mod->numFuncImports;
			for (unsigned i = 0; i < mod->numFunctions; i++)
				action(WasmFunction(&mod->functions[i]));
		}
	}

	LogHelper& operator<<(LogHelper& helper, const WasmError& error)
	{
		helper << "Error: " << error.info.result
			<< " (" << error.info.message << ")";
		if (error.info.file)
			helper << " at " << error.info.file << ":" << error.info.line;
		return helper;
	}
}