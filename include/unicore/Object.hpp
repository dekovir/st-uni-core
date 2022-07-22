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
		using ParentType = Parent; \
		static TypeConstRef type_static() { \
			static const auto s_type = internal::create<Base, Parent>(#Base); \
			return s_type; } \
	private:

#define UC_OBJECT(Base, Parent) \
		UC_OBJECT_BASE(Base, Parent) \
		UC_NODISCARD TypeConstRef type() const override { return type_static(); } \
	private:

	class Object
	{
		UC_OBJECT_BASE(Object, Object)
	public:
		virtual ~Object() = default;

		UC_NODISCARD virtual TypeConstRef type() const { return type_static(); }
	};
}