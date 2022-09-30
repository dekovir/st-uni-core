#include "FileLoader.hpp"

namespace unicore
{
	Shared<Resource> ReadFileLoader::load(const Context& context)
	{
		return _provider.open_read(context.path);
	}

	Shared<Resource> WriteFileLoader::load(const Context& context)
	{
		return _provider.create_new(context.path);
	}
}