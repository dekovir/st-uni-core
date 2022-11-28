#pragma once
#include "unicore/ui/elements/ElementContainer.hpp"
#include "unicore/resource/DataModel.hpp"

namespace unicore::ui
{
	// GroupElement //////////////////////////////////////////////////////////////
	template<typename TScheme,
		std::enable_if_t<std::is_base_of_v<GroupSchemeNode, TScheme>>* = nullptr>
	class GroupElement : public ElementContainer
	{
		UC_UI_ELEMENT_PROPERTY(name, String);
		UC_UI_ELEMENT_PROPERTY(hidden, Bool);
	public:
		GroupElement() = default;

		template<typename ... Args,
			std::enable_if_t<all_is_element_v<Args...>>* = nullptr>
		explicit GroupElement(Args&&... args)
		{
			((add(std::forward<Args>(args))), ...);
		}

		Shared<SchemeNode> render() override
		{
			auto temp = ptr(TScheme());
			apply_params(*temp);

			for (const auto& element : _elements)
				temp->add(element->render());

			return temp;
		}

		virtual void apply_params(TScheme& item)
		{
			if (_name.empty())
				item.set_params({ attr::Name(_name) });
			item.set_params({ attr::Visible(!_hidden) });
		}
	};

	using vlayout = GroupElement<GroupV>;
	using hlayout = GroupElement<GroupH>;
	using list_box = GroupElement<GroupList>;

	// ValueGroupElement /////////////////////////////////////////////////////////
	template<typename TScheme, typename TValue,
		std::enable_if_t<std::is_base_of_v<GroupSchemeNode, TScheme>>* = nullptr>
	class ValueGroupElement : public GroupElement<TScheme>
	{
	public:
		using ValueType = TValue;
		using ValueArg = sfinae::ConstRefType<ValueType>;

		ValueGroupElement() = default;

		template<typename ... Args,
			std::enable_if_t<all_is_element_v<Args...>>* = nullptr>
		explicit ValueGroupElement(Args&&... args)
		{
			((GroupElement<TScheme>::add(std::forward<Args>(args))), ...);
		}

		void set_value(ValueArg value)
		{
			if (_value == value) return;

			_value = value;
			GroupElement<TScheme>::rebuild();
		}

	protected:
		TValue _value;

		void apply_params(TScheme& item) override
		{
			GroupElement<TScheme>::apply_params(item);

			item.set_params({ attr::Value(_value) });
		}
	};

	template<typename T>
	using combo_box_t = ValueGroupElement<GroupCombo, T>;
	using combo_box = combo_box_t<Variant>;

	template<typename TValue>
	class GroupValueElement : public Element
	{
		UC_OBJECT_EVENT(changed, sfinae::ConstRefType<TValue>);
	public:
		using ValueType = TValue;
		using ValueArg = sfinae::ConstRefType<ValueType>;

		explicit GroupValueElement(ValueArg value)
			: _value(value)
		{}

		UC_NODISCARD ValueArg value() const { return _value; }

		void set_value(ValueArg value)
		{
			if (_value == value) return;

			_value = value;
			_event_changed(_value);
			rebuild();
		}

	protected:
		ValueType _value;
	};

	// RadioGroupElement /////////////////////////////////////////////////////////
	template<typename TKey>
	class RadioGroupElement : public GroupValueElement<TKey>
	{
	public:
		using ModelType = DictionaryDataModel<TKey, String32>;

		RadioGroupElement() = default;

		explicit RadioGroupElement(const Shared<ModelType>& model, TKey value)
			: GroupValueElement<TKey>(value), _model(model)
		{}

		UC_NODISCARD const Shared<ModelType>& model() const { return _model; }

		void set_model(const Shared<ModelType>& model)
		{
			if (_model == model) return;

			_model = model;
			GroupValueElement<TKey>::rebuild();
		}

		Shared<SchemeNode> render() override
		{
			auto group = ptr(GroupV());

			if (_model)
			{
				for (unsigned i = 0; i < _model->size(); i++)
				{
					auto key = _model->get_at(i);
					String32 value;
					if (_model->try_get_value(key, value))
					{
						const auto selected = GroupValueElement<TKey>::_value == key;
						group->add(GroupH(
							VisualText({ attr::Text(value) }),
							InputRadio({ attr::Value(selected), attr::OnChange([&]
								{ GroupValueElement<TKey>::set_value(key); }) })));
					}
				}
			}

			return group;
		}

	protected:
		Shared<ModelType> _model;
	};

	template<typename TKey>
	using radio_group_t = RadioGroupElement<TKey>;
	using radio_group = radio_group_t<Int>;

	// ModelViewElement //////////////////////////////////////////////////////////
	using ListModel = DataModel1<Shared<Element>>;
	using TableModel = TableDataModel<Shared<Element>>;

	template<typename TModel,
		std::enable_if_t<std::is_base_of_v<DataModel, TModel>>* = nullptr>
	class ModelViewElement : public Element
	{
	public:
		using ModelType = TModel;

		UC_NODISCARD const Shared<TModel>& model() const { return _model; }

		void set_model(const Shared<TModel>& model)
		{
			if (_model == model) return;

			_model = model;
			rebuild();
		}

	protected:
		Shared<TModel> _model;
	};

	// ListViewElement ///////////////////////////////////////////////////////////
	class ListModelViewElement : public ModelViewElement<ListModel>
	{
	public:
		ListModelViewElement() = default;

		explicit ListModelViewElement(const Shared<ModelType>& model)
		{
			set_model(model);
		}

		Shared<SchemeNode> render() override
		{
			auto list = ptr(GroupList());

			if (_model)
			{
				for (unsigned i = 0; i < _model->size(); i++)
				{
					if (const auto child = _model->get_at(i))
					{
						if (auto element = child->render())
							list->add(element);
					}
				}
			}

			return list;
		}
	};

	using list = ListModelViewElement;

	// TableModelViewElement /////////////////////////////////////////////////////
	class TableModelViewElement : public ModelViewElement<TableModel>
	{
	public:
		TableModelViewElement() = default;

		explicit TableModelViewElement(const Shared<ModelType>& model)
		{
			set_model(model);
		}

		Shared<SchemeNode> render() override
		{
			if (!_model) return nullptr;

			auto& size = _model->size();

			auto body = ptr(GroupTable({ attr::Value(size.x) }));

			// Header row
			if (const auto row = body->add(GroupTableRow()))
			{
				for (unsigned i = 0; i < size.x; i++)
				{
					auto text = String32(_model->get_header(i));
					row->add(GroupTableHeader({ attr::Text(text) }));
				}
			}

			// Cells
			for (unsigned y = 0; y < size.y; y++)
			{
				if (const auto row = body->add(GroupTableRow()))
				{
					for (unsigned x = 0; x < size.x; x++)
					{
						if (const auto cell = row->add(GroupTableCell()))
						{
							if (const auto element = _model->get_at({ x, y }))
							{
								if (auto item = element->render())
									cell->add(item);
							}
						}
					}
				}
			}

			return body;
		}
	};

	using table = TableModelViewElement;
}