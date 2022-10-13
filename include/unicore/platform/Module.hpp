#pragma once
#include "unicore/system/Object.hpp"
#include "unicore/system/Event.hpp"

namespace unicore
{
	class ModuleContainer;

	struct ModuleContext
	{
		Logger* logger;
		ModuleContainer& modules;
	};

	class Module : public Object
	{
		UC_OBJECT(Module, Object)
	public:
		virtual void register_module(UC_UNUSED const ModuleContext& context) {}
		virtual void unregister_module(UC_UNUSED const ModuleContext& context) {}
	};

	class ModuleContainer
	{
		UC_OBJECT_EVENT(add, Module&);
	public:
		void add(Module& module);

		UC_NODISCARD Module* find(TypeConstRef type) const;

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Module, T>>* = nullptr>
		UC_NODISCARD T* find() const
		{
			return dynamic_cast<T*>(find(get_type<T>()));
		}

		template<typename ... T,
			std::enable_if_t<(... && std::is_base_of_v<Module, T>)>* = nullptr>
		auto finds() const
		{
			return std::make_pair(find<T>()...);
		}

		void register_all(const ModuleContext& context);
		void unregister_all();

	protected:
		Set<Module*> _modules;
		Dictionary<TypeConstPtr, Module*> _types;

		const ModuleContext* _context = nullptr;
	};
}