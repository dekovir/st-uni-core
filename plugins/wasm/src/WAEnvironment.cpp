#include "unicore/wasm/WAEnvironment.hpp"
#include "unicore/Logger.hpp"
#include "unicore/wasm/WARuntime.hpp"
#include "unicore/wasm/WAModule.hpp"

namespace unicore
{
	WAEnvironment::WAEnvironment(IM3Environment handle, Logger& logger)
		: WAObject(handle), _logger(logger)
	{
	}

	WAEnvironment::~WAEnvironment()
	{
		if (_handle != nullptr)
			m3_FreeEnvironment(_handle);
	}

	Shared<WAModule> WAEnvironment::parse_module(const Shared<MemoryChunk>& memory) const
	{
		IM3Module mod;
		const auto error = m3_ParseModule(_handle, &mod, memory->data(), memory->size());
		if (!error)
			return std::make_shared<WAModule>(mod, memory, _logger);

		UC_LOG_ERROR(_logger) << error;
		return nullptr;
	}

	Shared<WARuntime> WAEnvironment::new_runtime(uint32_t stack_size)
	{
		if (auto runtime = m3_NewRuntime(_handle, stack_size, nullptr))
			return std::make_shared<WARuntime>(runtime, _logger);

		UC_LOG_ERROR(_logger) << "m3_NewRuntime failed";
		return nullptr;
	}

	Shared<WAEnvironment> WAEnvironment::create(Logger& logger)
	{
		if (auto env = m3_NewEnvironment())
			return std::make_shared<WAEnvironment>(env, logger);

		UC_LOG_ERROR(logger) << "m3_NewEnvironment failed";
		return nullptr;
	}
}