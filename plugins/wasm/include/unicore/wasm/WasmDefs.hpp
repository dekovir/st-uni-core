#pragma once
#include "unicore/Object.hpp"
#include <wasm3.h>

namespace unicore
{
	template<typename T>
	class WasmValue
	{
	public:
		explicit WasmValue(T handle)
			: _handle(handle) {}

		T handle() const { return _handle; }

	protected:
		T _handle;
	};

	template<typename T>
	class WasmObject : public WasmValue<T>, public Object
	{
	public:
		explicit WasmObject(T handle)
			: WasmValue<T>(handle)
		{
		}
	};

	// From https://github.com/wasm3/wasm3/blob/main/platforms/cpp/wasm3_cpp/include/wasm3_cpp.h
	namespace detail {
		typedef uint64_t* stack_type;
		typedef void* mem_type;
		template<typename T, typename...> struct first_type { typedef T type; };

		typedef const void* (*m3_api_raw_fn)(IM3Runtime, uint64_t*, void*);

		template<typename T>
		void arg_from_stack(T& dest, stack_type& _sp, mem_type mem) {
			m3ApiGetArg(T, tmp);
			dest = tmp;
		}

		template<typename T>
		void arg_from_stack(T*& dest, stack_type& _sp, mem_type _mem) {
			m3ApiGetArgMem(T*, tmp);
			dest = tmp;
		};

		template<typename T>
		void arg_from_stack(const T*& dest, stack_type& _sp, mem_type _mem) {
			m3ApiGetArgMem(const T*, tmp);
			dest = tmp;
		};

		template<char c>
		struct m3_sig {
			static const char value = c;
		};
		template<typename T> struct m3_type_to_sig;
		template<> struct m3_type_to_sig<int32_t> : m3_sig<'i'> {};
		template<> struct m3_type_to_sig<int64_t> : m3_sig<'I'> {};
		template<> struct m3_type_to_sig<float> : m3_sig<'f'> {};
		template<> struct m3_type_to_sig<double> : m3_sig<'F'> {};
		template<> struct m3_type_to_sig<void> : m3_sig<'v'> {};
		template<> struct m3_type_to_sig<void*> : m3_sig<'*'> {};
		template<> struct m3_type_to_sig<const void*> : m3_sig<'*'> {};

		template<typename Ret, typename ... Args>
		struct m3_signature {
			constexpr static size_t n_args = sizeof...(Args);
			constexpr static const char value[n_args + 4] = {
							m3_type_to_sig<Ret>::value,
							'(',
							m3_type_to_sig<Args>::value...,
							')',
							0
			};
		};

		template <typename ...Args>
		static void get_args_from_stack(stack_type& sp, mem_type mem, std::tuple<Args...>& tuple) {
			std::apply([&](auto &... item) {
				(arg_from_stack(item, sp, mem), ...);
				}, tuple);
		}

		template<typename Func>
		struct wrap_helper;

		template <typename Ret, typename ...Args>
		struct wrap_helper<Ret(Args...)> {
			using Func = Ret(Args...);
			static const void* wrap_fn(IM3Runtime rt, IM3ImportContext _ctx, stack_type _sp, mem_type mem) {
				std::tuple<Args...> args;
				// The order here matters: m3ApiReturnType should go before calling get_args_from_stack,
				// since both modify `_sp`, and the return value on the stack is reserved before the arguments.
				m3ApiReturnType(Ret);
				get_args_from_stack(_sp, mem, args);
				Func* function = reinterpret_cast<Func*>(_ctx->userdata);
				Ret r = std::apply(function, args);
				m3ApiReturn(r);
			}
		};

		template <typename ...Args>
		struct wrap_helper<void(Args...)> {
			using Func = void(Args...);
			static const void* wrap_fn(IM3Runtime rt, IM3ImportContext _ctx, stack_type sp, mem_type mem) {
				std::tuple<Args...> args;
				get_args_from_stack(sp, mem, args);
				Func* function = reinterpret_cast<Func*>(_ctx->userdata);
				std::apply(function, args);
				m3ApiSuccess();
			}
		};

		template<typename Func>
		class m3_wrapper;

		template<typename Ret, typename ... Args>
		class m3_wrapper<Ret(Args...)> {
		public:
			static M3Result link(IM3Module io_module,
				const char* const i_moduleName,
				const char* const i_functionName,
				Ret(*function)(Args...)) {
				return m3_LinkRawFunctionEx(io_module, i_moduleName, i_functionName,
					m3_signature<Ret, Args...>::value,
					&wrap_helper<Ret(Args...)>::wrap_fn,
					reinterpret_cast<void*>(function));
			}
		};
	} // namespace detail
}