#pragma once
#include "unicore/ui/UIComponent.hpp"

namespace unicore::ui
{
	class EditorObjectProperty
	{
	public:
		virtual ~EditorObjectProperty() = default;

		UC_NODISCARD virtual const TypeInfo& type() const = 0;
		UC_NODISCARD virtual StringView name() const = 0;

		virtual void set(const Variant& value) = 0;
		UC_NODISCARD virtual Variant get() const = 0;
	};

	template<typename TObject, typename TValue>
	class EditorObjectPropertyVariable : public EditorObjectProperty
	{
	public:
		using TypeValue = TValue;
		using TypeArg = unicore::sfinae::ConstRefType<TypeValue>;

		using VarType = TypeValue TObject::*;

		EditorObjectPropertyVariable(StringView name, TObject* obj, VarType var)
			: _name(name), _object(obj), _var(var) {}

		UC_NODISCARD const TypeInfo& type() const override { return get_type_info<TypeValue>(); }
		UC_NODISCARD StringView name() const override { return _name; }

		void set(const Variant& value) override
		{
			TypeValue type_value;
			if (value.try_get(type_value))
				set_type(type_value);
		}

		UC_NODISCARD Variant get() const override
		{
			return get_type();
		}

		TypeValue get_type() const
		{
			return _object->*_var;
		}

		void set_type(TypeArg value)
		{
			_object->*_var = value;
		}

	protected:
		String _name;
		TObject* _object;
		VarType _var;
	};

	template<typename TValue>
	class EditorObjectPropertyFunc : public EditorObjectProperty
	{
	public:
		using TypeValue = TValue;
		using TypeArg = unicore::sfinae::ConstRefType<TypeValue>;

		using GetFunc = std::function<TypeValue()>;
		using SetFunc = std::function<void(TypeArg)>;

		EditorObjectPropertyFunc(StringView name,
			const GetFunc& get, const SetFunc& set)
			: _name(name), _get(get), _set(set) {}

		UC_NODISCARD const TypeInfo& type() const override { return get_type_info<TypeValue>(); }
		UC_NODISCARD StringView name() const override { return _name; }

		void set(const Variant& value) override
		{
			set_type(value.get<TypeValue>());
		}

		UC_NODISCARD Variant get() const override
		{
			return get_type();
		}

		TypeValue get_type() const { return _get; }
		void set_type(TypeArg value) { _set(value); }

	protected:
		String _name;
		GetFunc _get;
		SetFunc _set;
	};

	class EditorObject
	{
	public:
		using PropertyList = List<Shared<EditorObjectProperty>>;

		explicit EditorObject(const Shared<Object>& object)
			: _object(object)
		{}

		UC_NODISCARD const Shared<Object>& object() const { return _object; }
		UC_NODISCARD const PropertyList& properties() const { return _properties; }

		void add(const Shared<EditorObjectProperty>& prop)
		{
			_properties.push_back(prop);
		}

	protected:
		PropertyList _properties;
		Shared<Object> _object;
	};

	template<typename TObject>
	class EditorObjectBuilder
	{
	public:
		explicit EditorObjectBuilder(const Shared<TObject>& object)
			: _object(object)
		{}

		template<typename TValue>
		EditorObjectBuilder& add(StringView name, TValue TObject::* var)
		{
			_properties.push_back(
				std::make_shared<EditorObjectPropertyVariable<TObject, TValue>>(name, _object.get(), var));
			return *this;
		}

		UC_NODISCARD Shared<EditorObject> build() const
		{
			auto editor = std::make_shared<EditorObject>(_object);
			for (const auto& prop : _properties)
				editor->add(prop);
			return editor;
		}

	protected:
		Shared<TObject> _object;
		List<Shared<EditorObjectProperty>> _properties;
	};

	class EditorComponent : public TestComponent
	{
	public:
	};

	class PropertyEditorComponent : public EditorComponent
	{
	public:
		UC_NODISCARD const Shared<EditorObjectProperty>& get_property() const { return _property; }

		void set_property(const Shared<EditorObjectProperty>& prop)
		{
			if (_property == prop) return;

			_property = prop;
		}

	protected:
		Shared<EditorObjectProperty> _property;
	};

	using PropertyEditorComponentFactory = std::function<Shared<PropertyEditorComponent>()>;

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	class IntPropertyEditorComponent : public PropertyEditorComponent
	{
	protected:
		Shared<Component> render() override
		{
			return ptr(input_int_t<T>(_property->get().get_integral<T>(),
				[&](auto value) { _property->set(value); }));
		}
	};

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	class FloatPropertyEditorComponent : public PropertyEditorComponent
	{
	protected:
		Shared<Component> render() override
		{
			auto start_value = _property->get().get_floating_point<T>();
			return ptr(input_float_t<T>(start_value,
				[&](auto value) { _property->set(value); }));
		}
	};

	class StringPropertyEditorComponent : public PropertyEditorComponent
	{
	protected:
		Shared<Component> render() override
		{
			const auto text = _property->get().get_string32();
			return ptr(input_text(text,
				[&](auto value) { _property->set(value); }));
		}
	};

#define UNICORE_UI_CONTEXT_REGISTER_INT_TYPE(context, type) \
	(context)->create<type, ui::IntPropertyEditorComponent<type>>()

	class ObjectEditorContext
	{
	public:
		ObjectEditorContext()
		{
#define REGISTER_INT(type) create<type, IntPropertyEditorComponent<type>>()
#define REGISTER_FLOAT(type) create<type, FloatPropertyEditorComponent<type>>()

			REGISTER_INT(Int8);
			REGISTER_INT(Int16);
			REGISTER_INT(Int32);
			REGISTER_INT(Int64);
			REGISTER_INT(UInt8);
			REGISTER_INT(UInt16);
			REGISTER_INT(UInt32);
			REGISTER_INT(UInt64);

			REGISTER_FLOAT(Float);
			REGISTER_FLOAT(Double);

#undef REGISTER_INT
#undef REGISTER_FLOAT
		}

		void add(const TypeInfo& type, const PropertyEditorComponentFactory& factory)
		{
			_factories[type] = factory;
		}

		template<typename TType, typename TEditor,
			std::enable_if_t<std::is_base_of_v<PropertyEditorComponent, TEditor>>* = nullptr>
		void create()
		{
			auto factory = [] { return std::make_shared<TEditor>(); };
			add(get_type_info<TType>(), factory);
		}

		UC_NODISCARD const PropertyEditorComponentFactory* find(const TypeInfo& type) const
		{
			const auto it = _factories.find(type);
			return it != _factories.end() ? &it->second : nullptr;
		}

	protected:
		Dictionary<TypeIndex, PropertyEditorComponentFactory> _factories;
	};

	class ObjectEditorComponent : public EditorComponent
	{
	public:
		explicit ObjectEditorComponent(ObjectEditorContext& context)
			: _context(context)
		{
		}

		void set_object(const Shared<EditorObject>& object)
		{
			_object = object;
		}

	protected:
		ObjectEditorContext& _context;
		Shared<EditorObject> _object;

		Shared<Component> render() override
		{
			if (!_object) return nullptr;

			auto layout = std::make_shared<vlayout>();

			for (const auto& prop : _object->properties())
			{
				const auto factory = _context.find(prop->type());
				if (!factory) continue;

				const auto component = (*factory)();

				if (!component) continue;

				component->set_property(prop);
				layout->add(ui::hlayout(
					ui::text(Unicode::to_utf32(prop->name())),
					component
				));
			}

			return layout;
		}
	};
}