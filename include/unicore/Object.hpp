#pragma once
#include "unicore/Type.hpp"

namespace unicore
{
	namespace internal
	{
		template<typename Base, typename Parent>
		static Type create(StringView classname)
		{
			auto parent = !std::is_same_v<Base, Parent> ? &get_type<Parent>() : nullptr;
			return Type(classname, parent, typeid(Base));
		}
	}

#define UC_OBJECT_BASE(Base, Parent) \
	public: \
		using ThisType = Base; \
		using ParentType = Parent; \
		static TypeConstRef type_static() { \
			static const auto s_type = internal::create<Base, Parent>(#Base); \
			return s_type; } \
	private:

#define UC_OBJECT(Base, Parent) \
		UC_OBJECT_BASE(Base, Parent) \
	public: \
		UC_NODISCARD TypeConstRef type() const override { return type_static(); } \
	private:

#define UC_OBJECT_EQUALS() \
	UC_NODISCARD bool equals(const Object& other) const override \
	{ \
		const auto options = dynamic_cast<const ThisType*>(&other); \
		return options ? equals(*options) : false; \
	} \
	UC_NODISCARD bool equals(const ThisType& other) const

	class Object
	{
		UC_OBJECT_BASE(Object, Object)
	public:
		virtual ~Object() = default;

		UC_NODISCARD virtual TypeConstRef type() const { return type_static(); }

		template<typename T>
		T* try_cast()
		{
			return type().can_cast_to<T>()
				? static_cast<T*>(this) : nullptr;
		}

		template<typename T>
		const T* try_cast() const
		{
			return type().can_cast_to<T>()
				? static_cast<const T*>(this) : nullptr;
		}

		UC_NODISCARD virtual bool equals(const Object& other) const
		{
			return this == &other;
		}
	};

	/*template<typename T, typename TA, typename TB,
		std::enable_if_t<std::is_base_of_v<Object, TA>>* = nullptr,
		std::enable_if_t<std::is_base_of_v<Object, TB>>* = nullptr>
	static bool operator==(const TA& a, const TB& b)
	{
		if (&a == &b) return true;

		if (a.type() == b.type())
			return a == b;

		return false;
	}*/
}