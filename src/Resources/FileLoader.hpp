#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/File.hpp"
#include "unicore/FileProvider.hpp"

namespace unicore
{
	class ReadFileLoader : public ResourceLoaderTyped<ReadFile, ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(ReadFileLoader, ResourceLoader)
	public:
		explicit ReadFileLoader(ReadFileProvider& provider)
			: _provider(provider)
		{
		}

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		ReadFileProvider& _provider;
	};

	class WriteFileLoader : public ResourceLoaderTyped<ReadFile, ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(WriteFileLoader, ResourceLoader)
	public:
		explicit WriteFileLoader(WriteFileProvider& provider)
			: _provider(provider)
		{
		}

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		WriteFileProvider& _provider;
	};
}