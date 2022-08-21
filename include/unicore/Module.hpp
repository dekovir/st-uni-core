#pragma once
#include "unicore/Context.hpp"

namespace unicore
{
	class Module : public Object
	{
		UC_OBJECT(Module, Object)
	public:
		virtual void register_module(UC_UNUSED Context& context) {}
		virtual void unregister_module(UC_UNUSED Context& context) {}
	};

	class ModuleContainer
	{
	public:
		void add(Module& module);

		UC_NODISCARD Module* find(TypeConstRef type) const;

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Module, T>>* = nullptr>
		UC_NODISCARD Module* find() const
		{
			return find(get_type<T>());
		}

		void register_all(Context& context);
		void unregister_all();

	protected:
		Dictionary<TypeConstPtr, Module*> _modules;
		Context* _context = nullptr;
	};
}