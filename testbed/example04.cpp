#include "example04.hpp"
#if defined(UNICORE_USE_WASM)
#include "unicore/Memory.hpp"
#include "unicore/Data.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/wasm/WAEnvironment.hpp"
#include "unicore/wasm/WAModule.hpp"
#include "unicore/wasm/WARuntime.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example04, "WASM");

	Example04::Example04(const ExampleContext& context)
		: Example(context)
		, _console(80, 20)
	{
	}

	bool Example04::load(ResourceCache& resources)
	{
		_font = resources.load<Font>(L"assets/font_004.fnt"_path);
		if (!_font) return false;

		ProxyLogger wasm_logger("[WASM] ", logger);
		if (const auto env = WAEnvironment::create(wasm_logger))
		{
			// ADD ///////////////////////////////////////////////////////////////////
			if (const auto data = resources.load<BinaryData>(L"assets/add.wasm"_path))
			{
				if (const auto runtime = env->new_runtime(); runtime)
				{
					if (const auto mod = env->parse_module(data->chunk()))
					{
						mod->load_to(*runtime);

						UC_LOG_INFO(wasm_logger) << "Runtime functions:";
						unsigned index = 0;
						runtime->enum_functions([&wasm_logger, &index](WAFunction func)
							{
								UC_LOG_INFO(wasm_logger) << index << ": " << func;
								index++;
							});

						if (const auto func = runtime->find_function("add"); func.has_value())
						{
							if (const auto error = func->call(10, 15); !error)
							{
								int v;
								func->get_results(&v);
								UC_LOG_INFO(wasm_logger) << "Function result: " << v;
							}
							else
							{
								UC_LOG_ERROR(wasm_logger) << error;
							}
						}
					}
				}
			}

			// COUNTER ///////////////////////////////////////////////////////////////
			if (const auto data = resources.load<BinaryData>(L"assets/counter.wasm"_path))
			{
				if (const auto runtime = env->new_runtime(); runtime)
				{
					if (const auto mod = env->parse_module(data->chunk()))
					{
						mod->load_to(*runtime);

						UC_LOG_INFO(wasm_logger) << "Runtime functions:";
						unsigned index = 0;
						runtime->enum_functions([&wasm_logger, &index](WAFunction func)
							{
								UC_LOG_INFO(wasm_logger) << index << ": " << func;
								index++;
							});

						const auto counter_get = runtime->find_function("counter_get");
						const auto counter_set = runtime->find_function("counter_set");
						const auto counter_inc = runtime->find_function("counter_inc");
						const auto counter_dec = runtime->find_function("counter_dec");

						auto mem = runtime->get_memory();

						counter_set->call(42);
						counter_inc->call();
						counter_dec->call();

						std::vector vec(mem.data(), mem.data() + mem.size());

						const auto result = counter_get->call_single<int>();
						UC_LOG_INFO(wasm_logger) << "Answer is " << result;
					}
				}
			}
		}

		return true;
	}

	void Example04::update()
	{
	}

	void Example04::draw() const
	{

	}
}
#endif