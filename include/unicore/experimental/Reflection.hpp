#pragma once
#include "unicore/Utility.hpp"

namespace unicore::Reflection
{
	class PropertyInfo;
	class MethodInfo;

	class Type
	{
	public:
		StringView classname;
		TypeIndex index;
		const Type* parent = nullptr;

		List<const PropertyInfo*> properties;
		List<const MethodInfo*> methods;

		UC_NODISCARD const PropertyInfo* find_property(StringView name) const;
		UC_NODISCARD const MethodInfo* find_method(StringView name) const;
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const Type&);

	struct ContextData;

	class Context
	{
	public:
		static void add(const Type& type);

		static const Type* find(TypeIndex index);

		static void dump(Logger& logger);

		template<typename T>
		static const Type* find()
		{
			return find(get_type_index<T>());
		}

		template<typename T>
		static const Type* find(const T& value)
		{
			return find(get_type_index(value));
		}

	protected:
		static ContextData* get_data();
	};

	template<typename T>
	extern const Type* try_get_type() { return Context::find(get_type_index<T>()); }

	template<typename T>
	extern const Type* try_get_type(const T& value) { return Context::find(get_type_index(value)); }

	template<typename T>
	extern const Type& get_type()
	{
		auto index = get_type_index<T>();
		auto type = Context::find(index);
		UC_ASSERT_MSG(type != nullptr, "Type not registered");
		return *type;
	}

	template<typename T>
	extern const Type& get_type(const T& value)
	{
		auto index = get_type_index(value);
		auto type = Context::find(index);
		UC_ASSERT_MSG(type != nullptr, "Type not registered");
		return *type;
	}

	class FieldInfo
	{
	public:
		const StringView name;

		explicit FieldInfo(StringView name_) : name(name_) {}
		~FieldInfo() = default;
	};

	class PropertyInfo : public FieldInfo
	{
	public:
		const Type& type;

		explicit PropertyInfo(const StringView& name_, const Type& type_)
			: FieldInfo(name_), type(type_)
		{
		}
	};
	extern UNICODE_STRING_BUILDER_FORMAT(const PropertyInfo&);

	template<typename ClassType, typename T>
	class PropertyValueInfo : public PropertyInfo
	{
	public:
		using ValueType = T ClassType::*;

		PropertyValueInfo(const StringView& name_, const ValueType value_)
			: PropertyInfo(name_, get_type<T>())
			, value(value_)
		{
		}

		ValueType value;
	};

	template<typename ClassType, typename T>
	class PropertyFuncInfo : public PropertyInfo
	{
	public:
		using GetType = const T& (ClassType::*)() const;
		using SetType = void (ClassType::*)(const T&);

		GetType get;
		SetType set;

		PropertyFuncInfo(const StringView& name_, const GetType get_, const SetType set_)
			: PropertyInfo(name_, get_type<T>()), get(get_), set(set_)
		{
		}
	};

	class MethodInfo : public FieldInfo
	{
	public:
		const Type* return_type;
		const List<const Type*> arg_types;

		explicit MethodInfo(StringView name_,
			const Type* return_type_, std::initializer_list<const Type*> args)
			: FieldInfo(name_), return_type(return_type_), arg_types(args)
		{
		}
	};
	extern UNICODE_STRING_BUILDER_FORMAT(const MethodInfo&);

	template<typename ClassType, typename Ret, typename ... Args>
	class MethodInfoT : public MethodInfo
	{
	public:
		using ValueType = Ret(ClassType::*)(Args...);

		MethodInfoT(StringView name_, const ValueType value_)
			: MethodInfo(name_, get_type<Ret>(), { get_type<Args>()... }), value(value_)
		{
		}

		ValueType value;
	};

	template<typename ClassType, typename Ret, typename ... Args>
	class ConstMethodInfoT : public MethodInfo
	{
	public:
		using ValueType = Ret(ClassType::*)(Args...) const;

		ConstMethodInfoT(StringView name_, const ValueType value_)
			: MethodInfo(name_, get_type<Ret>(), { get_type<Args>()... }), value(value_)
		{
		}

		ValueType value;
	};

	template<typename T>
	class RegisterFundamental
	{
	public:
		explicit RegisterFundamental(StringView classname)
		{
			_type = new Type{ classname, get_type_index<T>(), nullptr, {}, {} };
		}

		~RegisterFundamental()
		{
			Context::add(*_type);
		}

	protected:
		Type* _type;
	};

	template<typename ClassType, typename ParentType = ClassType>
	class RegisterClass
	{
	public:
		explicit RegisterClass(StringView classname)
		{
			_type = new Type{ classname, get_type_index<ClassType>(), nullptr, {}, {} };
			if (!std::is_same_v<ClassType, ParentType>)
				_type->parent = &get_type<ParentType>();
		}

		~RegisterClass()
		{
			Context::add(*_type);
		}

		template<typename Ret, typename ... Args>
		RegisterClass& method(StringView name,
			typename MethodInfoT<ClassType, Ret, Args...>::ValueType func)
		{
			auto info = new MethodInfoT<ClassType, Ret, Args...>(name, func);
			_type->methods.push_back(info);
			return *this;
		}

		template<typename Ret, typename ... Args>
		RegisterClass& method(StringView name,
			typename ConstMethodInfoT<ClassType, Ret, Args...>::ValueType func)
		{
			auto info = new ConstMethodInfoT<ClassType, Ret, Args...>(name, func);
			_type->methods.push_back(info);
			return *this;
		}

		template<typename T>
		RegisterClass& property(StringView name, T ClassType::* prop)
		{
			auto info = new PropertyValueInfo<ClassType, T>(name, prop);
			_type->properties.push_back(info);
			return *this;
		}

		template<typename T>
		RegisterClass& property(StringView name,
			typename PropertyFuncInfo<ClassType, T>::GetType get,
			typename PropertyFuncInfo<ClassType, T>::SetType set = nullptr)
		{
			auto info = new PropertyFuncInfo<ClassType, T>(name, get, set);
			_type->properties.push_back(info);
			return *this;
		}

	protected:
		Type* _type;
	};

#define UNICORE_REFLECTION_REGISTER_CLASS(type) \
	Reflection::RegisterClass<type>(#type)

#define UNICORE_REFLECTION_REGISTER() \
	UNICORE_EXECUTE_CODE_BLOCK(reflection)
}