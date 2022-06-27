#include "unicore/Data.hpp"
#include "unicore/Stream.hpp"
#include "unicore/LogHelper.hpp"

namespace unicore
{
	Shared<Resource> BinaryDataLoader::load(const ResourceLoaderContext& context)
	{
		const auto size = static_cast<size_t>(context.stream.size());
		context.stream.seek(0);
		auto buffer = Memory::alloc(size);
		const auto readed = context.stream.read(buffer, 1, size);
		if (readed == size)
			return make_shared<BinaryData>(buffer, size);

		free(buffer);
		UC_LOG_ERROR(context.logger) << "Read failed";
		return nullptr;
	}

	Shared<Resource> TextDataLoader::load(const ResourceLoaderContext& context)
	{
		const auto size = static_cast<size_t>(context.stream.size());
		context.stream.seek(0);
		String str;
		str.resize(size);
		context.stream.read(str.data(), 1, size);
		return std::make_shared<TextData>(str);
	}
}