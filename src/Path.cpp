#include "unicore/Path.hpp"
#include <cwctype>
#include <algorithm>
#if defined (UNICORE_PLATFORM_WINDOWS)
#	include <Shlwapi.h>
#endif
#include "unicore/Unicode.hpp"
#include "unicore/StringHelper.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	const Path Path::Empty;

	constexpr Char Path::DirSeparator = '/';

#if defined (ENG_PLATFORM_WIN)
	constexpr Char Path::NativeDirSeparator = '\\';
#else
	constexpr Char Path::NativeDirSeparator = '/';
#endif

	constexpr Char WrongDirSeparator = '\\';

	Path::Path()
		: _hash(0)
	{
	}

	Path::Path(StringView path)
		: _data(prepare(path))
	{
		_hash = calc_hash(_data);
	}

	Path::Path(StringView16 path)
		: _data(prepare(Unicode::to_utf8(path)))
	{
		_hash = calc_hash(_data);
	}

	Path::Path(StringView32 path)
		: _data(prepare(Unicode::to_utf8(path)))
	{
		_hash = calc_hash(_data);
	}

	Path::Path(Path&& other) noexcept
		: _data(std::move(other._data))
		, _hash(std::exchange(other._hash, 0))
	{
	}

	Path::Path(StringView path, std::size_t hash)
		: _data(path), _hash(hash)
	{
	}

	Path& Path::operator=(Path&& other) noexcept
	{
		_data = std::move(other._data);
		_hash = std::exchange(other._hash, 0);
		return *this;
	}

	bool Path::absolute() const
	{
		// TODO: Test
#if defined (UNICORE_PLATFORM_WINDOWS)
		return _data.size() > 2 && _data[1] == L':' && _data[2] == DirSeparator;
#else
		return !_data.empty() && _data[0] == DirSeparator;
#endif
	}

	void Path::clear()
	{
		_data.clear();
		_hash = 0;
	}

	bool Path::has_extension() const
	{
		const auto pos = find_extension_pos(_data);
		return pos != String::npos;
	}

	bool Path::has_extension(StringView ext) const
	{
		// TODO: Optimize
		return extension() == ext;
	}

	bool Path::starts_with(const Path& path) const
	{
		return StringHelper::starts_with(
			StringView(_data), StringView(path._data));
	}

	void Path::parent_path(Path& parentPath) const
	{
		const auto pos = find_filename_pos(_data);
		if (pos != String::npos)
			parentPath = Path(_data.substr(0, pos));
		else parentPath.clear();
	}

	Path Path::parent_path() const
	{
		Path str;
		parent_path(str);
		return str;
	}

	void Path::filename(String& fileName) const
	{
		const auto pos = find_filename_pos(_data);
		if (pos != String::npos)
			fileName = _data.substr(pos + 1);
		else fileName = _data;
	}

	String Path::filename() const
	{
		String str;
		filename(str);
		return str;
	}

	void Path::extension(String& ext) const
	{
		const auto pos = find_extension_pos(_data);
		if (pos != String::npos)
			ext = _data.substr(pos);
		else ext.clear();
	}

	String Path::extension() const
	{
		String str;
		extension(str);
		return str;
	}

	void Path::explode(Path& parentDir, String& fileName) const
	{
		const auto pos = find_filename_pos(_data);
		if (pos != String::npos)
		{
			parentDir = Path(_data.substr(0, pos));
			fileName = _data.substr(pos + 1);
		}
		else
		{
			parentDir = Empty;
			fileName = _data;
		}
	}

	void Path::explode(Path& parentDir, String& fileName, String& extension) const
	{
		explode(parentDir, fileName);

		const auto pos = find_extension_pos(fileName);
		if (pos != std::u32string::npos)
		{
			extension = fileName.substr(pos);
			fileName = fileName.substr(0, pos);
		}
		else extension = {};
	}

	void Path::explode(List<String>& elements) const
	{
		String::size_type prev = 0, pos;
		while ((pos = _data.find(DirSeparator, prev)) != String::npos)
		{
			const auto count = pos - prev;
			if (count > 0)
				elements.push_back(_data.substr(prev, count));
			prev = pos + 1;
		}

		elements.push_back(_data.substr(prev));
	}

	List<String> Path::explode() const
	{
		List<String> elements;
		explode(elements);
		return elements;
	}

	void Path::replace_filename(StringView filename)
	{
		const auto pos = find_filename_pos(_data);
		if (pos != String::npos)
		{
			_data = _data.substr(0, pos + 1);
			_data += filename;
		}
		else
		{
			_data = filename;
		}

		_hash = calc_hash(_data);
	}

	void Path::replace_extension(StringView ext)
	{
		if (const auto pos = find_extension_pos(_data); pos != String::npos)
			_data = _data.substr(0, pos);
		else if (ext.empty())
			return;

		_data += ext;
		_hash = calc_hash(_data);
	}

	void Path::remove_extension()
	{
		if (const auto pos = find_extension_pos(_data); pos != String::npos)
		{
			_data = _data.substr(0, pos);
			_hash = calc_hash(_data);
		}
	}

	bool Path::equals(const Path& other) const
	{
		return _hash == other._hash || _data == other._data;
	}

	int Path::compare(const Path& other) const
	{
		if (_hash < other._hash)
			return -1;
		if (_hash > other._hash)
			return +1;

		return _data == other._data;
	}

	Path Path::operator/(const String& file) const
	{
		if (file.empty())
			return *this;

		if (empty())
			return Path(file);

		return combine(_data, file);
	}

	Path Path::operator/(const Path& path) const
	{
		if (path.empty())
			return *this;

		if (empty())
			return path;

		return combine(_data, path._data);
	}

	void Path::native_path(String& nativePath) const
	{
		nativePath = _data;
		if constexpr (DirSeparator != NativeDirSeparator)
		{
			std::u32string::size_type pos;
			while ((pos = nativePath.find(DirSeparator)) != std::u32string::npos)
				nativePath[pos] = NativeDirSeparator;
		}
	}

	String Path::native_path() const
	{
		String str;
		native_path(str);
		return str;
	}

	Path Path::to_lower() const
	{
#if defined (UNICORE_PLATFORM_WINDOWS)
		return Path(_data, _hash);
#else
		auto tmp = _data;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), towlower);
		return Path(tmp);
#endif
	}

	Path Path::combine(StringView a, StringView b)
	{
		if (a.empty() && b.empty())
			return Empty;

		if (a.empty())
			return Path(b);

		if (b.empty())
			return Path(a);

		String builder;
		builder.reserve(a.size() + b.size() + 1);
		builder += a;
		builder += DirSeparator;
		builder += b;
		return Path(builder);
	}

	Path Path::combine(StringView a, StringView b, StringView c)
	{
		String builder;
		builder.reserve(a.size() + b.size() + 1);
		builder += a;
		builder += DirSeparator;
		builder += b;
		builder += DirSeparator;
		builder += c;
		return Path(builder);
	}

	// =========================================================================
	String::size_type Path::find_filename_pos(StringView str)
	{
		return str.find_last_of(DirSeparator);
	}

	String::size_type Path::find_extension_pos(StringView str)
	{
		// TODO: Fix for './test' case
		return str.find_last_of(U'.');
	}

	String Path::prepare(const StringView _path)
	{
		static const String DoubleDirSeparator
			= { DirSeparator, DirSeparator };

		auto path = String(_path);
#if defined (UNICORE_PLATFORM_WINDOWS)
		{
			//wchar_t BUFFER[MAX_PATH];
			//// TODO: Optimize
			//if (PathCanonicalizeW(BUFFER, path.data()))
			//	path = BUFFER;
		}
#endif
		size_t pos;
		while ((pos = path.find(WrongDirSeparator)) != String::npos)
			path[pos] = DirSeparator;

		while ((pos = path.find(DoubleDirSeparator)) != String::npos)
			path = path.replace(pos, DoubleDirSeparator.size(), 1, DirSeparator);

		const auto endPos = path.find_last_not_of(DirSeparator);
		if (endPos + 1 < path.size())
			path = path.substr(0, endPos + 1);

#if defined (UNICORE_PLATFORM_WINDOWS)
		std::transform(path.begin(), path.end(), path.begin(), towlower);
#endif

		return path;
	}

	size_t Path::calc_hash(const StringView str)
	{
		static constexpr HashFunc<String> hash_func;
		return !str.empty() ? hash_func(str.data()) : 0;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Path&)
	{
		return builder << '\'' << value.data() << '\'';
	}
}