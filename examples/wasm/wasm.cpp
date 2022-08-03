#include "wasm.hpp"
#include "UnicoreMain.h"
#include "unicore/Time.hpp"
#include "unicore/Timer.hpp"
#include "unicore/Data.hpp"
#include "unicore/Input.hpp"
#include "unicore/Strings.hpp"
#include "unicore/TimeSpan.hpp"
#include "unicore/RendererSDL.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/wasm/WAEnvironment.hpp"
#include "unicore/wasm/WAModule.hpp"
#include "unicore/wasm/WARuntime.hpp"
#include "unicore/xml/XMLPlugin.hpp"
#include "unicore/fnt/FNTPlugin.hpp"
#include <locale>
#include <codecvt>

namespace unicore
{
	class State
	{
	public:
		explicit State(WARuntime& runtime)
		{
			_init = runtime.find_function("app_init");
			_update = runtime.find_function("app_update");
			_draw = runtime.find_function("app_draw");
		}

		void init()
		{
			if (_init.has_value())
				_init->call();
		}

		void update(const TimeSpan& delta)
		{
			if (_update.has_value())
				_update->call((float)delta.total_seconds());
		}

		void draw()
		{
			if (_draw.has_value())
				_draw->call();
		}

	protected:
		Optional<WAFunction> _init;
		Optional<WAFunction> _update;
		Optional<WAFunction> _draw;
	};

	static MyCore* s_example = nullptr;
	static Shared<State> s_state;
	static TimeSpan s_state_time = TimeSpanConst::Zero;

	m3ApiRawFunction(wa_abort)
	{
		m3ApiGetArg(int32_t, gas);
		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_log)
	{
		m3ApiGetArgMem(const uint32_t*, ptr);
		const uint32_t lenInPoints = *(ptr - 1) / 2;

		const std::basic_string_view strUtf16(reinterpret_cast<const char16_t*>(ptr), lenInPoints);
		const auto text = Strings::to_utf8(strUtf16);

		s_example->logger.write(LogType::Info, text);

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_random)
	{
		m3ApiReturnType(uint32_t);
		m3ApiReturn(s_example->_random.next());

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_random_f)
	{
		m3ApiReturnType(float);
		m3ApiReturn(s_example->_random.next_float());

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_screen_x)
	{
		auto& size = s_example->renderer.screen_size();

		m3ApiReturnType(int);
		m3ApiReturn(size.x);

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_screen_y)
	{
		auto& size = s_example->renderer.screen_size();

		m3ApiReturnType(int);
		m3ApiReturn(size.y);

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_mouse_x)
	{
		auto& pos = s_example->input.mouse().position();

		m3ApiReturnType(int);
		m3ApiReturn(pos.x);

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_mouse_y)
	{
		auto& pos = s_example->input.mouse().position();

		m3ApiReturnType(int);
		m3ApiReturn(pos.y);

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_mouse_button)
	{
		m3ApiReturnType(int);
		m3ApiReturn(s_example->input.mouse().down() ? 1 : 0);

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_sprite_draw_at)
	{
		m3ApiGetArg(float, x);
		m3ApiGetArg(float, y);

		s_example->_sprite_batch.draw(s_example->_spr, Vector2f(x, y));
		s_example->_sprite_count++;

		m3ApiSuccess();
	}

	MyCore::MyCore(const CoreSettings& settings)
		: SDLCore(create_settings(settings, "Wasm"))
		, _console(80, 20)
	{
		UC_LOG_INFO(logger) << "Starting";

		s_example = this;

		add_plugin<XMLPlugin>();
		add_plugin<FNTPlugin>();

		_font = resources.load<Font>(L"font_004.fnt"_path);

		_font = resources.load<Font>(L"font_004.fnt"_path);
		if (auto tex = resources.load<Texture>(L"zazaka.png"_path))
			_spr = std::make_shared<Sprite>(tex);

		ConsoleLogger wasm_logger(_console);
		_env = WAEnvironment::create(wasm_logger);

		UC_LOG_INFO(wasm_logger) << "Loading release.wasm";
		if (const auto data = resources.load<BinaryData>(L"logic.wasm"_path))
		{
			_runtime = _env->new_runtime();
			if (_runtime)
			{
				_module = _env->parse_module(data->chunk());
				if (_module)
				{
					_module->load_to(*_runtime);
					_module->link_function_raw("env", "abort", "v(iiii)", &wa_abort);
					_module->link_function_raw("index", "log", "v(i)", &wa_log);
					_module->link_function_raw("index", "random", "i()", &wa_random);
					_module->link_function_raw("index", "random_f", "f()", &wa_random_f);

					_module->link_function_raw("index", "screen_x", "i()", &wa_screen_x);
					_module->link_function_raw("index", "screen_y", "i()", &wa_screen_y);

					_module->link_function_raw("index", "mouse_x", "i()", &wa_mouse_x);
					_module->link_function_raw("index", "mouse_y", "i()", &wa_mouse_y);
					_module->link_function_raw("index", "mouse_button", "i()", &wa_mouse_button);

					_module->link_function_raw("index", "sprite_draw_at", "v(ff)", &wa_sprite_draw_at);

					UC_LOG_INFO(wasm_logger) << "Runtime functions:";
					unsigned index = 0;
					_runtime->enum_functions([&wasm_logger, &index](WAFunction func)
						{
							UC_LOG_INFO(wasm_logger) << index << ": " << func;
							index++;
						});

					s_state = std::make_shared<State>(*_runtime);
				}
			}
		}

		s_state->init();

		resources.unload_unused();
	}

	MyCore::~MyCore()
	{
		s_example = nullptr;
		s_state = nullptr;
	}

	void MyCore::on_update()
	{
#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.quit();
#endif

		_sprite_batch.begin();

		// UPDATE STATE
		s_state_time += time.delta();

		{
			AutoTimer timer(_update_time);
			static constexpr auto fps_lock = TimeSpan::from_seconds(1. / 60.);
			while (s_state_time > fps_lock)
			{
				s_state->update(fps_lock);
				s_state_time -= fps_lock;
			}
		}

		// DRAW STATE
		_sprite_count = 0;
		{
			AutoTimer timer(_draw_time);
			s_state->draw();
		}

		// DRAW CONSOLE
		const auto size = _console.size();
		for (int y = 0; y < size.y; y++)
			for (int x = 0; x < size.x; x++)
			{
				DefaultConsoleChar c;
				if (!_console.get(x, y, c))
					continue;

				if (c.c == 0 || c.color == 0)
					continue;

				const Vector2f pos(x * 10, y * 20 + 100);
				const StringView str(&c.c, 1);
				const Color4b color(
					c.color.has(ConsoleColor8::ForegroundRed) ? 255 : 0,
					c.color.has(ConsoleColor8::ForegroundGreen) ? 255 : 0,
					c.color.has(ConsoleColor8::ForegroundBlue) ? 255 : 0
				);
				_sprite_batch.print(_font, pos, str, color);
			}

		// DRAW TEXT
		const String fps_str = "FPS: " + std::to_string(fps());
		const String sprites_str = "Sprites: " + std::to_string(_sprite_count);
		const String update_str = "Update time: " + std::to_string(_update_time.total_milliseconds()) + "ms";
		const String draw_str = "Draw time: " + std::to_string(_draw_time.total_milliseconds()) + "ms";

		_sprite_batch.print(_font, { 0, 0 }, fps_str);
		_sprite_batch.print(_font, { 0, 20 }, sprites_str);
		_sprite_batch.print(_font, { 0, 40 }, update_str);
		_sprite_batch.print(_font, { 0, 60 }, draw_str);

		_sprite_batch.end();
	}

	void MyCore::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		_sprite_batch.render(renderer);
	}

	UNICORE_MAIN_CORE(MyCore);
}