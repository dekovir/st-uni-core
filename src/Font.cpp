#include "unicore/Font.hpp"
#include "unicore/Data.hpp"
#include "unicore/LogHelper.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/BinaryReader.hpp"

namespace unicore
{
	struct BlockInfo
	{
		int16_t font_size;
		uint8_t bit_field;
		uint8_t char_set;
		uint16_t stretch_h;
		uint8_t aa;
		uint8_t padding_up;
		uint8_t padding_right;
		uint8_t padding_down;
		uint8_t padding_left;
		uint8_t spacing_h;
		uint8_t spacing_v;
		uint8_t outline;
		char* file_name;
	};

	bool BitmapFontLoader::can_load_extension(WStringView ext) const
	{
		return ext == L".fnt";
	}

	Shared<Resource> BitmapFontLoader::load(const ResourceLoaderContext& context)
	{
		uint8_t guard[3];

		BinaryReader reader(context.stream);

		context.stream.seek(0);
		if (context.stream.read(guard, 1, 3) != 3)
		{
			UC_LOG_ERROR(context.logger) << "Failed to read guard";
			return nullptr;
		}

		if (guard[0] != 66 || guard[1] != 77 || guard[2] != 70)
		{
			UC_LOG_WARNING(context.logger) << "Wrong format header guard";
			return nullptr;
		}

		uint8_t version;
		if (!reader.read(version))
		{
			UC_LOG_ERROR(context.logger) << "Failed to read version";
			return nullptr;
		}

		if (version != 3)
		{
			UC_LOG_ERROR(context.logger) << version << "is invalid version format";
			return nullptr;
		}

		while (!reader.eof())
		{
			uint32_t block_size;
			reader.read(block_size);
		}

		Path texture_path(context.path);
		texture_path.remove_extension();

		//const auto texture = context.cache.load<Texture>(context.path);
		//if (!texture)
		//{
		//	UC_LOG_WARNING(context.logger) << "Failed to load Texture";
		//	return nullptr;
		//}

		return nullptr;
	}
}