#include "unicore/StreamProvider.hpp"

namespace unicore
{
	bool BasicStreamProvider::exists(const Path& path)
	{
		return stats(path).has_value();
	}
}
