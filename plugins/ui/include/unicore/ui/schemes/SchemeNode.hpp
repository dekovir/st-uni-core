#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "unicore/ui/UINodeValues.hpp"

namespace unicore::ui
{
	class SchemeNode
	{
	public:
		explicit SchemeNode(UINodeTag tag);
		virtual ~SchemeNode() = default;

		UC_NODISCARD UINodeTag tag() const { return _tag; }

		virtual UINode create(UIDocument& document, const UINode& parent) const;
		virtual Bool apply_to(UIDocument& document, const UINode& node) const;

	protected:
		const UINodeTag _tag;

		virtual void fill_optons(UINodeOptions& options) const = 0;
	};

	// SchemeNodeKeys ////////////////////////////////////////////////////////////
	template<typename ... Keys>
	class SchemeNodeKeys
	{
	public:
		using ValuesType = UINodeValuesT<Keys...>;
		using Params = typename ValuesType::Params;

		void set_params(Params params)
		{
			_values.apply(params);
		}

	protected:
		ValuesType _values;
	};

	// SFINAE ////////////////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Bool is_scheme_node_v =
		std::is_base_of_v<SchemeNode, T> ||
		std::is_convertible_v<T, Shared<SchemeNode>>;

	template<typename ... T>
	inline constexpr Bool all_is_scheme_node_v = (... && is_scheme_node_v<T>);

	// UTILITIES /////////////////////////////////////////////////////////////////
	template<typename T,
		std::enable_if_t<std::is_base_of_v<SchemeNode, T>>* = nullptr>
	extern Shared<T> ptr(const T& item)
	{
		return std::make_shared<T>(item);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<SchemeNode, T>>* = nullptr>
	extern Shared<T> ptr(T&& item)
	{
		return std::make_shared<T>(std::forward<T>(item));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<SchemeNode, T>>* = nullptr>
	extern Shared<T> ref(const T& item, Shared<T>& ref)
	{
		ref = ptr(item);
		return ref;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<SchemeNode, T>>* = nullptr>
	extern Shared<T> ref(T&& item, Shared<T>& ref)
	{
		ref = ptr(item);
		return ref;
	}
}