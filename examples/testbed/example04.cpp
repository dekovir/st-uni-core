#include "example04.hpp"
#if defined(UNICORE_USE_WASM)
#include "unicore/Memory.hpp"
#include "unicore/Time.hpp"
#include "unicore/Data.hpp"
#include "unicore/Input.hpp"
#include "unicore/TimeSpan.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/Renderer2D.hpp"
#include "unicore/wasm/WAEnvironment.hpp"
#include "unicore/wasm/WAModule.hpp"
#include "unicore/wasm/WARuntime.hpp"
#include <locale>
#include <codecvt>

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example04, "WASM");

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

	static Example04* s_example = nullptr;
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
		uint32_t lenInPoints = *(ptr - 1) / 2;

		std::u16string strUtf16(reinterpret_cast<const char16_t*>(ptr), 0, lenInPoints);
		std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
		auto text = converter.to_bytes(strUtf16);

		s_example->logger.write(LogType::Info, text);

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_random)
	{
		m3ApiReturnType(uint32_t);
		m3ApiReturn(s_example->_random.next());

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
		auto pressed = s_example->input.mouse().down();

		m3ApiReturnType(int);
		m3ApiReturn(pressed ? 1 : 0);

		m3ApiSuccess();
	}

	m3ApiRawFunction(wa_sprite_draw_at)
	{
		m3ApiGetArg(float, x);
		m3ApiGetArg(float, y);

		s_example->_sprite_batch.draw(s_example->_spr, Vector2f(x, y));

		m3ApiSuccess();
	}

	Example04::Example04(const ExampleContext& context)
		: Example(context)
		, _console(80, 20)
	{
		s_example = this;
	}

	Example04::~Example04()
	{
		s_example = nullptr;
		s_state = nullptr;
	}

	bool Example04::load(ResourceCache& resources)
	{
		_font = resources.load<Font>(L"assets/font_004.fnt"_path);
		auto tex = resources.load<Texture>(L"assets/zazaka.png"_path);
		if (tex)
			_spr = std::make_shared<Sprite>(tex);

		if (!_font || !_spr) return false;

		ConsoleLogger wasm_logger(_console);
		_env = WAEnvironment::create(wasm_logger);
		if (!_env)
			return false;

		UC_LOG_INFO(wasm_logger) << "Loading release.wasm";
		if (const auto data = resources.load<BinaryData>(L"assets/release.wasm"_path))
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

		return true;
	}

	void Example04::update()
	{
		_sprite_batch.begin();

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

		s_state_time += time.delta();

		static constexpr auto fps_lock = TimeSpan::from_seconds(1./30.);
		while (s_state_time > fps_lock)
		{
			s_state->update(fps_lock);
			s_state_time -= fps_lock;
		}

		s_state->draw();

		_sprite_batch.end();
	}

	void Example04::draw() const
	{
		_sprite_batch.render(renderer);
	}
}
#endif