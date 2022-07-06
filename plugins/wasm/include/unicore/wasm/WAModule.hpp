#pragma once
#include "unicore/wasm/WADefs.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class Logger;
	class WARuntime;

	class WAGlobal : public WAValue<IM3Global>
	{
	public:
		explicit WAGlobal(IM3Global handle)
			: WAValue(handle)
		{
		}

		UC_NODISCARD M3ValueType get_type() const
		{
			return m3_GetGlobalType(_handle);
		}
	};

	class WAModule : public WAObject<IM3Module>
	{
	public:
		WAModule(IM3Module handle, const Shared<MemoryChunk>& memory, Logger& logger);
		~WAModule();

		UC_NODISCARD StringView name() const { return m3_GetModuleName(_handle); }
		void set_name(StringView name);

		bool link_function_raw(StringView module_name,
			StringView function_name, StringView signature, M3RawCall func);

		UC_NODISCARD WAGlobal find_global(StringView name) const;

		bool load_to(WARuntime& runtime);

		bool compile();
		bool run_start();

	protected:
		Shared<MemoryChunk> _memory;
		WARuntime* _runtime = nullptr;
		Logger& _logger;
	};
}