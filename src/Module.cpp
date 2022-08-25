#include "unicore/Module.hpp"

namespace unicore
{
	static const auto ModuleType = get_type<Module>();

	void ModuleContainer::add(Module& module)
	{
		_modules.insert(&module);

		for (auto type = &module.type(); type != nullptr && type != &ModuleType; type = type->parent)
		{
			_types.emplace(type, &module);
		}

		if (_context)
		{
			module.register_module(*_context);
			_event_add(module);
		}
	}

	Module* ModuleContainer::find(TypeConstRef type) const
	{
		const auto it = _types.find(&type);
		return it != _types.end() ? it->second : nullptr;
	}

	void ModuleContainer::register_all(const ModuleContext& context)
	{
		if (_context) return;

		_context = &context;
		for (const auto mod : _modules)
			mod->register_module(*_context);
	}

	void ModuleContainer::unregister_all()
	{
		if (!_context) return;

		for (const auto mod : _modules)
			mod->unregister_module(*_context);
		_context = nullptr;
	}
}