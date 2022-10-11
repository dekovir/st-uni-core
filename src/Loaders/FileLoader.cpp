#include "unicore/loaders/FileLoader.hpp"

namespace unicore
{
	// ReadFileLoader /////////////////////////////////////////////////////////////
	ReadFileLoader::ReadFileLoader(ReadFileProvider& provider)
		: _provider(provider)
	{
	}

	Shared<Resource> ReadFileLoader::load(const Context& context)
	{
		return _provider.open_read(context.path);
	}

	// WriteFileLoader ////////////////////////////////////////////////////////////
	WriteFileLoader::WriteFileLoader(WriteFileProvider& provider)
		: _provider(provider)
	{
	}

	Shared<Resource> WriteFileLoader::load(const Context& context)
	{
		return _provider.create_new(context.path);
	}
}