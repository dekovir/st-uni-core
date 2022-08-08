#pragma once
#include "unicore/wasm/WasmDefs.hpp"
#include "unicore/BinaryData.hpp"

namespace unicore
{
	class Logger;
	class WasmRuntime;
	class WasmModule;

	class WasmEnvironment : public WasmObject<IM3Environment>
	{
	public:
		WasmEnvironment(IM3Environment handle, Logger& logger);
		~WasmEnvironment() override;

		UC_NODISCARD Shared<WasmModule> parse_module(const Shared<BinaryData>& data) const;

		Shared<WasmRuntime> new_runtime(uint32_t stack_size = 1024);

		static Shared<WasmEnvironment> create(Logger& logger);

	protected:
		Logger& _logger;
	};
}