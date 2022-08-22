#include "unicore/Context.hpp"
#include "unicore/Logger.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	void Context::register_type(TypeConstRef type)
	{
		if (!_types.insert(&type).second)
			return;

		if (const auto parent = type.parent)
		{
			_derived_types[parent].insert(&type);
			register_type(*parent);
		}
	}

	void Context::dump_types(Logger& logger) const
	{
		UC_LOG_INFO(logger) << "Context dump types: " << _types.size();
		unsigned index = 0;
		for (const auto& type : _types)
		{
			StringBuilder builder;
			builder << index << ": " << type->classname;
			if (type->parent)
				builder << " (" << type->parent->classname << ")";

			if (auto it = _derived_types.find(type); it != _derived_types.end())
			{
				builder << " -> [";
				unsigned i = 0;
				for (const auto& derived_type : it->second)
				{
					if (i > 0) builder << ',';
					builder << derived_type->classname;
					i++;
				}
				builder << "]";
			}

			UC_LOG_INFO(logger) << builder;

			index++;
		}
	}
}