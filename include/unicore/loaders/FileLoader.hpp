#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/File.hpp"
#include "unicore/FileProvider.hpp"

namespace unicore
{
	class ReadFileLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<ReadFile>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(ReadFileLoader, ResourceLoader)
	public:
		explicit ReadFileLoader(ReadFileProvider& provider);

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		ReadFileProvider& _provider;
	};

	class WriteFileLoader : public ResourceLoaderOptionsTyped<WriteFileOptions,
		ResourceLoaderTypePolicy::Multiple<WriteFile>,
		ResourceLoaderPathPolicy::NotEmpty,
		ResourceLoaderOptionsPolicy::NullOrExact<WriteFileOptions>>
	{
		UC_OBJECT(WriteFileLoader, ResourceLoader)
	public:
		explicit WriteFileLoader(WriteFileProvider& provider);

		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const WriteFileOptions& options) override;

	protected:
		WriteFileProvider& _provider;
	};
}