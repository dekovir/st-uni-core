#pragma once
#include "unicore/wasm/WasmFunction.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class Logger;
	class WasmModule;

	struct WasmError
	{
		M3ErrorInfo info;
	};

	class WasmRuntime : public WasmObject<IM3Runtime>
	{
	public:
		WasmRuntime(IM3Runtime handle, Logger& logger);
		~WasmRuntime() override;

		UC_NODISCARD MemoryChunk get_memory() const;
		UC_NODISCARD WasmError get_error() const;

		UC_NODISCARD Optional<WasmFunction> find_function(StringView function_name) const;

		void enum_functions(const Action<WasmFunction>& action) const;

	protected:
		Logger& _logger;
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const WasmError&);
}