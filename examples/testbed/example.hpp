#pragma once
#include "unicore/Logger.hpp"
#include "unicore/Object.hpp"

namespace unicore
{
	class Logger;
	class Random;
	class Time;
	class Input;
	class Renderer2D;
	class ResourceCache;

	struct ExampleContext
	{
		Logger& logger;
		Random& random;
		Time& time;
		Input& input;
		Renderer2D& renderer;
	};

	class Example : public Object
	{
	public:
		Logger& logger;
		Random& random;
		Time& time;
		Input& input;
		Renderer2D& renderer;

		explicit Example(const ExampleContext& context);

		virtual bool load(ResourceCache& resources) = 0;

		virtual void update() = 0;
		virtual void draw() const = 0;
	};

	using ExampleFactory = std::function<Shared<Example>(const ExampleContext& context)>;

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
		{ return std::make_shared<type>(context); })
}