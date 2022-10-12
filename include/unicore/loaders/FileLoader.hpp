#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/io/File.hpp"
#include "unicore/io/FileProvider.hpp"

namespace unicore
{
	class ReadFileLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<ReadFile>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(ReadFileLoader, ResourceLoader)
	public:
		explicit ReadFileLoader(ReadFileProvider& provider);

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		ReadFileProvider& _provider;
	};

	class WriteFileLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<WriteFile>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(WriteFileLoader, ResourceLoader)
	public:
		explicit WriteFileLoader(WriteFileProvider& provider);

		Shared<Resource> load(const Context& context) override;

	protected:
		WriteFileProvider& _provider;
	};
}