#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class ResourcePath
	{
	public:
		constexpr ResourcePath(StringView base, StringView data, StringView args = "")
			: _base(base), _data(data), _args(args)
		{}

		UC_NODISCARD constexpr StringView base() const { return _base; }
		UC_NODISCARD constexpr StringView data() const { return _data; }
		UC_NODISCARD constexpr StringView args() const { return _args; }

		UC_NODISCARD String to_string() const
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

		static constexpr ResourcePath make(const StringView str)
		{
			const auto base_pos = str.find_first_of(BaseSeparator);

			const auto base =
				base_pos != StringView::npos ? str.substr(0, base_pos) : StringView();
			const auto tmp =
				base_pos != StringView::npos ? str.substr(base_pos + BaseSeparator.size()) : str;

			const auto args_pos = tmp.find_first_of(ArgsSeparator);
			const auto data =
				args_pos != StringView::npos ? tmp.substr(0, args_pos) : tmp;
			const auto args =
				args_pos != StringView::npos ? tmp.substr(args_pos + ArgsSeparator.size()) : StringView();

			return { base , data, args };
		}

		static constexpr StringView BaseSeparator = ":";
		static constexpr StringView ArgsSeparator = "?";

	protected:
		StringView _base;
		StringView _data;
		StringView _args;
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
}