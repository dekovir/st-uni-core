#pragma once
#include "unicore/wasm/WasmDefs.hpp"
#include "unicore/BinaryData.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Resource.hpp"

namespace unicore
{
	class WasmRuntime;

	class WasmModule : public WasmObject<IM3Module, Resource>
	{
	public:
		WasmModule(IM3Module handle, const Shared<BinaryData>& data, Logger& logger);
		~WasmModule() override;

		UC_NODISCARD size_t get_system_memory_use() const override;

		UC_NODISCARD StringView get_name() const;
		void set_name(StringView name);

		bool link_function_raw(StringView module_name,
			StringView function_name, StringView signature, M3RawCall func);

		template<typename Func>
		bool link_module(StringView module_name, StringView function_name, Func* function, bool optional = false) {
			const M3Result ret = detail::m3_wrapper<Func>::link(_handle, module_name.data(), function_name.data(), function);
			if (optional && ret == m3Err_functionLookupFailed)
				return false;

			if (ret != m3Err_none)
			{
				UC_LOG_ERROR(_logger) << ret;
				return false;
			}

			return true;
		}

		template<typename Func>
		bool link(StringView function_name, Func* function, bool optional = false) {
			return link_module("*", function_name, function, optional);
		}

		bool load_to(WasmRuntime& runtime);

		bool compile();
		bool run_start();

	protected:
		Shared<BinaryData> _data;
		WasmRuntime* _runtime = nullptr;
		Logger& _logger;
	};
}