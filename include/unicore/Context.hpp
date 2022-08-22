#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	class Logger;
	class Resource;
	class ResourceLoader;
	class ResourceConverter;

	class Context
	{
	public:
		void register_type(TypeConstRef type);
		void dump_types(Logger& logger) const;

	protected:
		Set<TypeConstPtr> _types;
		Dictionary<TypeConstPtr, Set<TypeConstPtr>> _derived_types;
	};
}