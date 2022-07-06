#pragma once
#include "unicore/wasm/WADefs.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class Logger;
	class WAModule;

	struct WAError
	{
		M3ErrorInfo info;
	};

	class WAFunction : public WAValue<IM3Function>
	{
	public:
		explicit WAFunction(IM3Function handle)
			: WAValue(handle) {}

		UC_NODISCARD StringView name() const { return m3_GetFunctionName(_handle); }

		void get_arguments(List<M3ValueType>& types) const
		{
			const auto count = m3_GetArgCount(_handle);
			for (unsigned i = 0; i < count; i++)
				types.push_back(m3_GetArgType(_handle, i));
		}

		void get_returns(List<M3ValueType>& types) const
		{
			const auto count = m3_GetRetCount(_handle);
			for (unsigned i = 0; i < count; i++)
				types.push_back(m3_GetRetType(_handle, i));
		}

		M3Result call() const
		{
			return m3_Call(_handle, 0, {});
		}

		template<typename Ret = void, typename... Args>
		Ret call(Args... args) const
		{
			const void* arg_ptrs[] = { reinterpret_cast<const void*>(&args)... };
			M3Result res = m3_Call(_handle, sizeof...(args), arg_ptrs);
			//detail::check_error(res);

			if constexpr (!std::is_void<Ret>::value) {
				Ret ret;
				const void* ret_ptrs[] = { &ret };
				res = m3_GetResults(_handle, 1, ret_ptrs);
				//detail::check_error(res);
				return ret;
			}
		}
	};

	class WARuntime : public WAObject<IM3Runtime>
	{
	public:
		WARuntime(IM3Runtime handle, Logger& logger);
		~WARuntime();

		MemoryChunk get_memory() const;
		UC_NODISCARD WAError get_error() const;

		Optional<WAFunction> find_function(StringView function_name) const;

	protected:
		Logger& _logger;
	};

	extern LogHelper& operator << (LogHelper& helper, const WAError& error);
}