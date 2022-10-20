#include "unicore/wasm/WasmEnvironment.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/wasm/WasmRuntime.hpp"
#include "unicore/wasm/WasmModule.hpp"

namespace unicore
{
	WasmEnvironment::WasmEnvironment(IM3Environment handle, Logger& logger)
		: WasmObject(handle), _logger(logger)
	{
	}

	WasmEnvironment::~WasmEnvironment()
	{
		if (_handle != nullptr)
			m3_FreeEnvironment(_handle);
	}

	Shared<WasmModule> WasmEnvironment::parse_module(const Shared<BinaryData>& data) const
	{
		IM3Module mod;
		const auto error = m3_ParseModule(_handle,
			&mod, data->data_as<uint8_t>(), data->size());
		if (!error)
			return std::make_shared<WasmModule>(mod, data, _logger);

		UC_LOG_ERROR(_logger) << error;
		return nullptr;
	}

	Shared<WasmRuntime> WasmEnvironment::new_runtime(uint32_t stack_size)
	{
		if (auto runtime = m3_NewRuntime(_handle, stack_size, nullptr))
			return std::make_shared<WasmRuntime>(runtime, _logger);

		UC_LOG_ERROR(_logger) << "m3_NewRuntime failed";
		return nullptr;
	}

	Shared<WasmEnvironment> WasmEnvironment::create(Logger& logger)
	{
		if (auto env = m3_NewEnvironment())
			return std::make_shared<WasmEnvironment>(env, logger);

		UC_LOG_ERROR(logger) << "m3_NewEnvironment failed";
		return nullptr;
	}
}