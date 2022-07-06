#pragma once
#include "unicore/wasm/WAFunction.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class Logger;
	class WAModule;

	struct WAError
	{
		M3ErrorInfo info;
	};

	class WARuntime : public WAObject<IM3Runtime>
	{
	public:
		WARuntime(IM3Runtime handle, Logger& logger);
		~WARuntime() override;

		MemoryChunk get_memory() const;
		UC_NODISCARD WAError get_error() const;

		Optional<WAFunction> find_function(StringView function_name) const;

		void enum_functions(const Action<WAFunction>& action) const;

	protected:
		Logger& _logger;
	};

	extern LogHelper& operator << (LogHelper& helper, const WAError& error);
}