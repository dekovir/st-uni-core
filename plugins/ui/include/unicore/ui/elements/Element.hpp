#pragma once
#include "unicore/ui/UISchemes.hpp"

namespace unicore::ui
{
	UNICORE_MAKE_INDEX_WITH_INVALID(ElementIndex, UInt16);

	class Element
	{
	public:
		Element();
		virtual ~Element() = default;

		UC_NODISCARD ElementIndex index() const { return _index; }
		UC_NODISCARD bool is_mounted() const { return _document != nullptr; }

		void mount(UIDocument& document, const UINode& parent);
		void unmount();
		void update();

		virtual Shared<SchemeNode> render() = 0;

	protected:
		const ElementIndex _index;

		// TODO: Optimize by size (Replace 2 pointers with 1 index)
		UIDocument* _document = nullptr;
		UINode _node = UINode::Empty;

		void rebuild();

		virtual void did_mount() {}
		virtual void will_umount() {}
		virtual void did_update() {}

	private:
		bool _need_rerender = true;
	};

	// SFINAE ////////////////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Bool is_element_v =
		std::is_base_of_v<Element, T> ||
		std::is_convertible_v<T, Shared<Element>>;

	template<typename ... T>
	inline constexpr Bool all_is_element_v = (... && is_element_v<T>);

	// UTILITIES /////////////////////////////////////////////////////////////////
	template<typename T,
		std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
	extern Shared<T> ptr(const T& item)
	{
		return std::make_shared<T>(item);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
	extern Shared<T> ptr(T&& item)
	{
		return std::make_shared<T>(std::forward<T>(item));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
	extern Shared<T> ref(const T& item, Shared<T>& ref)
	{
		ref = ptr(item);
		return ref;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
	extern Shared<T> ref(T&& item, Shared<T>& ref)
	{
		ref = ptr(item);
		return ref;
	}

#define UC_UI_ELEMENT_PROPERTY(name, type) \
	public: UC_NODISCARD sfinae::ConstRefType<type> name() const { return _ ## name; } \
	void set_ ## name(sfinae::ConstRefType<type> value) \
	{ if (_ ## name == value) return; _ ## name = value; rebuild(); } \
	protected: type _ ## name = {}
}