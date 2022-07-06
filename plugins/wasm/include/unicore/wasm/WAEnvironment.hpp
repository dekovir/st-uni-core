#pragma once
#include "unicore/wasm/WADefs.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class Logger;
	class WARuntime;
	class WAModule;

	class WAEnvironment : public WAObject<IM3Environment>
	{
	public:
		WAEnvironment(IM3Environment handle, Logger& logger);
		~WAEnvironment() override;

		UC_NODISCARD Shared<WAModule> parse_module(const Shared<MemoryChunk>& memory) const;

		Shared<WARuntime> new_runtime(uint32_t stack_size = 1024);

		static Shared<WAEnvironment> create(Logger& logger);

	protected:
		Logger& _logger;
	};
}