#pragma once
#include "unicore/ui/UIDocument.hpp"

namespace unicore::ui
{
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		UC_NODISCARD UIDocument* document() { return _document; }
		UC_NODISCARD const UIDocument* document() const { return _document; }

		UC_NODISCARD const UINode& parent() const { return _parent; }

		UC_NODISCARD Bool is_mounted() const { return _document != nullptr; }

		virtual void mount(UIDocument& document, const UINode& parent);
		virtual void dismount();

	protected:
		virtual void on_mount() = 0;
		virtual void on_dismount() = 0;

	private:
		UIDocument* _document = nullptr;
		UINode _parent = UINode::Empty;
	};

	using component = Component;

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern auto ptr(T&& element)
	{
		return std::make_shared<T>(std::forward<T>(element));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern auto ptr(const Shared<T>& element)
	{
		return element;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern Shared<T> ref(T&& element, Shared<T>& value)
	{
		value = std::make_shared<T>(std::forward<T>(element));
		return value;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern Shared<T> ref(const Shared<T>& element, Shared<T>& value)
	{
		value = element;
		return value;
	}

	class TestComponent : public Component
	{
	public:

	protected:
		void on_mount() override
		{
			if (!_created)
				_created = render();

			_created->mount(*document(), parent());
		}

		void on_dismount() override
		{
			_created->dismount();
		}

		virtual Shared<Component> render() = 0;

	private:
		Shared<Component> _created;
	};
}

namespace unicore
{
	namespace sfinae
	{
		template<typename T>
		inline constexpr Bool is_component_v =
			std::is_base_of_v<ui::Component, T> ||
			std::is_convertible_v<T, Shared<ui::Component>>;

		template<typename ... T>
		inline constexpr Bool all_is_component_v = (... && is_component_v<T>);
	}
}