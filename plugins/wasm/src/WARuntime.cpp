#include "unicore/wasm/WARuntime.hpp"

#include <m3_env.h>

#include "unicore/Logger.hpp"
#include "unicore/wasm/WAModule.hpp"

namespace unicore
{
	WARuntime::WARuntime(IM3Runtime handle, Logger& logger)
		: WAObject(handle), _logger(logger)
	{
	}

	WARuntime::~WARuntime()
	{
		m3_FreeRuntime(_handle);
	}

	MemoryChunk WARuntime::get_memory() const
	{
		uint32_t size;
		auto data = m3_GetMemory(_handle, &size, 0);
		if (data) return MemoryChunk(data, size, false);

		UC_LOG_ERROR(_logger) << "m3_GetMemory failed";
		return {};
	}

	WAError WARuntime::get_error() const
	{
		WAError error{};
		m3_GetErrorInfo(_handle, &error.info);
		return error;
	}

	Optional<WAFunction> WARuntime::find_function(StringView function_name) const
	{
		IM3Function func;
		if (const auto error = m3_FindFunction(&func, _handle, function_name.data()); !error)
			return WAFunction(func);

		return std::nullopt;
	}

	void WARuntime::enum_functions(const Action<WAFunction>& action) const
	{
		for (auto mod = _handle->modules; mod != nullptr; mod = mod->next)
		{
			const auto count = mod->numFunctions + mod->numFuncImports;
			for (unsigned i = 0; i < count; i++)
				action(WAFunction(&mod->functions[i]));
		}
	}

	LogHelper& operator<<(LogHelper& helper, const WAError& error)
	{
		helper << "Error: " << error.info.result
			<< " (" << error.info.message << ")";
		if (error.info.file)
			helper << " at " << error.info.file << ":" << error.info.line;
		return helper;
	}
}