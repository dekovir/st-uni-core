#include "unicore/Module.hpp"

namespace unicore
{
	void ModuleContainer::add(Module& module)
	{
		_modules.emplace(&module.type(), &module);

		if (_context)
		{
			module.register_module(*_context);
			_event_add(module);
		}
	}

	Module* ModuleContainer::find(TypeConstRef type) const
	{
		const auto it = _modules.find(&type);
		return it != _modules.end() ? it->second : nullptr;
	}

	void ModuleContainer::register_all(const ModuleContext& context)
	{
		if (_context) return;

		_context = &context;
		for (const auto [type, mod] : _modules)
			mod->register_module(*_context);
	}

	void ModuleContainer::unregister_all()
	{
		if (!_context) return;

		for (const auto [type, mod] : _modules)
			mod->unregister_module(*_context);
		_context = nullptr;
	}
}