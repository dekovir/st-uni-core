#include "testbed.hpp"
#include "UnicoreMain.h"
#include "unicore/Input.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Data.hpp"
#include "unicore/Surface.hpp"
#include "unicore/wasm/WAEnvironment.hpp"
#include "unicore/wasm/WAModule.hpp"
#include "unicore/wasm/WARuntime.hpp"

namespace unicore
{
	MyCore::MyCore(const CoreSettings& settings)
		: Core(settings)
	{
		UC_LOG_INFO(logger) << "Starting";

		_font = resources.load<BitmapFont>(L"assets/font_004.fnt"_path);

		// wasm
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

		set_example(0);
	}

	void MyCore::on_update()
	{
#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.quit();
#endif

		if (input.keyboard().down_change(KeyCode::Space))
			set_example((_example_index + 1) % ExampleCatalog::get_all().size());

		if (_example)
			_example->update();

		// SPRITE BATCH ////////////////////////////////////////////////////////////
		const String title_str = "Example: " + ExampleCatalog::get_all()[_example_index].title;
		const String fps_str = "FPS: " + std::to_string(fps());
		const String draw_str = "Draw: " + std::to_string(_draw_calls);

		_sprite_batch.begin();

		_sprite_batch.print(_font, { 0, 0 }, fps_str);
		_sprite_batch.print(_font, { 200, 0 }, title_str);
		_sprite_batch.print(_font, { 0, 20 }, draw_str);

		_sprite_batch.end();
	}

	void MyCore::on_draw()
	{
		render.clear(ColorConst4b::Black);

		if (_example)
			_example->draw();

		_sprite_batch.render(render);

		_draw_calls = render.draw_calls();
	}

	void MyCore::set_example(int index)
	{
		if (_example_index == index) return;

		auto& info = ExampleCatalog::get_all()[index];

		const auto example = info.factory({ logger, _random, time, input, render });
		if (!example->load(resources))
			return;

		_example_index = index;
		resources.unload_unused();
		_example = example;
	}

	UNICORE_MAIN_CORE(MyCore);
}