#pragma once
#include "unicore/wasm/WasmDefs.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class Logger;
	class WasmRuntime;

	class WasmModule : public WasmObject<IM3Module>
	{
	public:
		WasmModule(IM3Module handle, const Shared<MemoryChunk>& memory, Logger& logger);
		~WasmModule() override;

		UC_NODISCARD StringView get_name() const;
		void set_name(StringView name);

		bool link_function_raw(StringView module_name,
			StringView function_name, StringView signature, M3RawCall func);

		bool load_to(WasmRuntime& runtime);

		bool compile();
		bool run_start();

	protected:
		Shared<MemoryChunk> _memory;
		WasmRuntime* _runtime = nullptr;
		Logger& _logger;
	};
}