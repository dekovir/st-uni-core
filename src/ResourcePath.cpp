#include "unicore/ResourcePath.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	String ResourcePath::to_string() const
	{
		if (!_base.empty())
		{
			String str;
			str.reserve(_base.size() + _data.size() + BaseSeparator.size());
			str += _base;
			str += BaseSeparator;
			str += _data;
			return str;
		}

		return String(_data);
	}

	LogHelper& operator<<(LogHelper& helper, const ResourcePath& path)
	{
		return helper << '\'' << path.to_string() << '\'';
	}
}