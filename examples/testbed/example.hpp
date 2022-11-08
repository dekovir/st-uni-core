#pragma once
#include "unicore/system/Object.hpp"
#include "unicore/math/Random.hpp"
#include "unicore/io/Path.hpp"
#include "unicore/renderer/sdl2/Pipeline.hpp"

namespace unicore
{
	class Platform;
	class Logger;
	class Random;
	class Time;
	class Input;
	class IResourceCache;
	class ImGuiContext;

	class Font;

	struct ExampleContext
	{
		Logger& logger;
		Random& random;
		Time& time;
		Input& input;
		sdl2::Pipeline& renderer;
		Platform& platform;
		Shared<Font> font;
		ImGuiContext& imgui;
	};

	class Example : public Object
	{
	public:
		Logger& logger;
		Random& random;
		Time& time;
		Input& input;
		sdl2::Pipeline& renderer;
		Platform& platform;

		explicit Example(const ExampleContext& context);

		virtual void load(IResourceCache& resources) {}
		virtual void update() = 0;
		virtual void draw() const = 0;

		virtual void get_text(List<String32>& lines) {}
		virtual void get_comment(String32& comment) {}

		virtual void on_drop_file(const Path& path) {}
	};

	using ExampleFactory = std::function<Unique<Example>(const ExampleContext& context)>;

	struct ExampleInfo
	{
		String title;
		ExampleFactory factory;
	};

	struct ExampleCatalog
	{
		static void add_factory(const ExampleInfo& info)
		{
			_examples.push_back(info);
		}

		static const List<ExampleInfo>& get_all() { return _examples; }

	private:
		static List<ExampleInfo> _examples;
	};

	class ExampleAutoRegister
	{
	public:
		const ExampleInfo info;

		ExampleAutoRegister(StringView title, const ExampleFactory& factory)
			: info({ title.data(), factory })
		{
			ExampleCatalog::add_factory(info);
		}
	};

#define UC_EXAMPLE_REGISTER(type, title) \
	static const ExampleAutoRegister s_example_register(title, [](const ExampleContext& context) \
		{ return std::make_unique<type>(context); })
}