#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class ResourcePath
	{
	public:
		constexpr ResourcePath(StringView base, StringView data)
			: _base(base), _data(data)
		{}

		UC_NODISCARD constexpr StringView base() const { return _base; }
		UC_NODISCARD constexpr StringView data() const { return _data; }

		UC_NODISCARD String to_string() const;

		static constexpr ResourcePath make(StringView const str)
		{
			const auto pos = str.find_first_of(BaseSeparator);
			if (pos != StringView::npos)
				return { str.substr(0, pos), str.substr(pos + BaseSeparator.size()) };

			return { "", str };
		}

		static constexpr StringView BaseSeparator = ":";

	protected:
		StringView _base;
		StringView _data;
	};

	static constexpr bool operator==(const ResourcePath& a, const ResourcePath& b)
	{
		return a.base() == b.base() && a.data() == b.data();
	}

	static constexpr bool operator!=(const ResourcePath& a, const ResourcePath& b)
	{
		return !(a == b);
	}

	static constexpr ResourcePath operator"" _res(const char* str, size_t len)
	{
		return ResourcePath::make(StringView(str, len));
	}

	class LogHelper;
	extern LogHelper& operator << (LogHelper& helper, const ResourcePath& path);
}