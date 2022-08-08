#pragma once
#include "unicore/wasm/WasmDefs.hpp"

namespace unicore
{
	class LogHelper;

	class WasmFunction : public WasmValue<IM3Function>
	{
	public:
		explicit WasmFunction(IM3Function handle);

		UC_NODISCARD StringView name() const;

		void get_arguments(List<M3ValueType>& types) const;
		void get_returns(List<M3ValueType>& types) const;

		M3Result call() const
		{
			return m3_Call(_handle, 0, {});
		}

		template<typename... Args>
		M3Result call(Args... args) const
		{
			const void* ptrs[] = { reinterpret_cast<const void*>(&args)... };
			return m3_Call(_handle, sizeof...(args), ptrs);
		}

		template<typename... Args>
		M3Result get_results(Args*... args) const
		{
			const void* ptrs[] = { reinterpret_cast<const void*>(args)... };
			return m3_GetResults(_handle, sizeof...(args), ptrs);
		}

		template<typename Ret, typename... Args>
		Ret call_single(Args... args) const
		{
			if constexpr (sizeof...(args) > 0)
			{
				const void* arg_ptrs[] = { reinterpret_cast<const void*>(&args)... };
				m3_Call(_handle, sizeof...(args), arg_ptrs);
			}
			else
			{
				m3_Call(_handle, 0, nullptr);
			}

			if constexpr (!std::is_void_v<Ret>) {
				Ret ret;
				const void* ret_ptrs[] = { &ret };
				m3_GetResults(_handle, 1, ret_ptrs);
				return ret;
			}
		}
	};

	extern UNICODE_STRING_BUILDER_FORMAT(M3ValueType);
	extern UNICODE_STRING_BUILDER_FORMAT(const WasmFunction&);
}