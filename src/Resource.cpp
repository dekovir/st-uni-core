#include "unicore/Resource.hpp"
#include "unicore/Hash.hpp"
namespace unicore
{
	ResourceOptionsTag::ResourceOptionsTag(StringView text)
		: text(text), hash_value(Crc32::compute(text))
	{}
}