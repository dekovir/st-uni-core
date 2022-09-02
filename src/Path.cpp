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

	constexpr wchar_t Path::DirSeparator = L'/';

#if defined (ENG_PLATFORM_WIN)
	constexpr wchar_t Path::NativeDirSeparator = L'\\';
#else
	constexpr wchar_t Path::NativeDirSeparator = L'/';
#endif

	constexpr wchar_t WrongDirSeparator = L'\\';
	constexpr wchar_t DriveSeparator = L':';

	Path::Path()
		: _hash(0)
	{
	}

	Path::Path(WStringView path)
		: _data(prepare(path))
	{
		_hash = calc_hash(_data);
	}

	Path::Path(Path&& other) noexcept
		: _data(std::move(other._data))
		, _hash(std::exchange(other._hash, 0))
	{
	}

	Path::Path(WStringView path, std::size_t hash)
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
		return find_drive_pos(_data) != WString::npos;
	}

	void Path::clear()
	{
		_data.clear();
		_hash = 0;
	}

	bool Path::has_extension() const
	{
		const auto pos = find_extension_pos(_data);
		return pos != WString::npos;
	}

	bool Path::has_extension(WStringView ext) const
	{
		// TODO: Optimize
		return extension() == ext;
	}

	void Path::parent_path(Path& parentPath) const
	{
		const auto pos = find_filename_pos(_data);
		if (pos != WString::npos)
			parentPath = Path(_data.substr(0, pos));
		else parentPath.clear();
	}

	Path Path::parent_path() const
	{
		Path str;
		parent_path(str);
		return str;
	}

	void Path::filename(WString& fileName) const
	{
		const auto pos = find_filename_pos(_data);
		if (pos != WString::npos)
			fileName = _data.substr(pos + 1);
		else fileName = _data;
	}

	WString Path::filename() const
	{
		WString str;
		filename(str);
		return str;
	}

	void Path::extension(WString& ext) const
	{
		const auto pos = find_extension_pos(_data);
		if (pos != std::u32string::npos)
			ext = _data.substr(pos);
		else ext.clear();
	}

	WString Path::extension() const
	{
		WString str;
		extension(str);
		return str;
	}

	void Path::explode(Path& parentDir, WString& fileName) const
	{
		const auto pos = find_filename_pos(_data);
		if (pos != WString::npos)
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

	void Path::explode(Path& parentDir, WString& fileName, WString& extension) const
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

	void Path::explode(List<WString>& elements) const
	{
		WString::size_type prev = 0, pos;
		while ((pos = _data.find(DirSeparator, prev)) != WString::npos)
		{
			const auto count = pos - prev;
			if (count > 0)
				elements.push_back(_data.substr(prev, count));
			prev = pos + 1;
		}

		elements.push_back(_data.substr(prev));
	}

	List<WString> Path::explode() const
	{
		List<WString> elements;
		explode(elements);
		return elements;
	}

	void Path::replace_filename(WStringView filename)
	{
		const auto pos = find_filename_pos(_data);
		if (pos != WString::npos)
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

	void Path::replace_extension(WStringView ext)
	{
		if (const auto pos = find_extension_pos(_data); pos != WString::npos)
			_data = _data.substr(0, pos);
		else if (ext.empty())
			return;

		_data += ext;
		_hash = calc_hash(_data);
	}

	void Path::remove_extension()
	{
		if (const auto pos = find_extension_pos(_data); pos != WString::npos)
		{
			_data = _data.substr(0, pos);
			_hash = calc_hash(_data);
		}
	}

	void Path::remove_extension(const List<WString>& extensions)
	{
		for (const auto& ext : extensions)
		{
			if (!StringHelper::ends_with<wchar_t>(_data, ext, true))
				continue;

			_data = _data.substr(0, _data.size() - ext.length());
			_hash = calc_hash(_data);
			break;
		}
	}

	bool Path::equals(const Path& other) const
	{
		return _hash == other._hash ||
			StringHelper::compare(_data, other._data, false) == 0;
	}

	int Path::compare(const Path& other) const
	{
		if (_hash < other._hash)
			return -1;
		if (_hash > other._hash)
			return +1;

		return StringHelper::compare(_data, other._data, false);
	}

	Path Path::operator/(const WString& file) const
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

	void Path::native_path(WString& nativePath) const
	{
		nativePath = _data;
		if constexpr (DirSeparator != NativeDirSeparator)
		{
			std::u32string::size_type pos;
			while ((pos = nativePath.find(DirSeparator)) != std::u32string::npos)
				nativePath[pos] = NativeDirSeparator;
		}
	}

	WString Path::native_path() const
	{
		WString str;
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

	Path Path::combine(WStringView a, WStringView b)
	{
		if (a.empty() && b.empty())
			return Empty;

		if (a.empty())
			return Path(b);

		if (b.empty())
			return Path(a);

		WString builder;
		builder.reserve(a.size() + b.size() + 1);
		builder += a;
		builder += DirSeparator;
		builder += b;
		return Path(builder);
	}

	Path Path::combine(WStringView a, WStringView b, WStringView c)
	{
		WString builder;
		builder.reserve(a.size() + b.size() + 1);
		builder += a;
		builder += DirSeparator;
		builder += b;
		builder += DirSeparator;
		builder += c;
		return Path(builder);
	}

	// =========================================================================
	WString::size_type Path::find_drive_pos(WStringView str)
	{
		return str.find_last_of(DriveSeparator);
	}

	WString::size_type Path::find_filename_pos(WStringView str)
	{
		return str.find_last_of(DirSeparator);
	}

	WString::size_type Path::find_extension_pos(WStringView str)
	{
		// TODO: Fix for './test' case
		return str.find_last_of(U'.');
	}

	WString Path::prepare(const WStringView _path)
	{
		static const WString DoubleDirSeparator
			= { DirSeparator, DirSeparator };

		auto path = WString(_path);
#if defined (UNICORE_PLATFORM_WINDOWS)
		{
			wchar_t BUFFER[MAX_PATH];
			// TODO: Optimize
			if (PathCanonicalizeW(BUFFER, path.data()))
				path = BUFFER;
		}
#endif
		size_t pos;
		while ((pos = path.find(WrongDirSeparator)) != WString::npos)
			path[pos] = DirSeparator;

		while ((pos = path.find(DoubleDirSeparator)) != WString::npos)
			path = path.replace(pos, DoubleDirSeparator.size(), 1, DirSeparator);

		const auto endPos = path.find_last_not_of(DirSeparator);
		if (endPos + 1 < path.size())
			path = path.substr(0, endPos + 1);

#if defined (UNICORE_PLATFORM_WINDOWS)
		std::transform(path.begin(), path.end(), path.begin(), towlower);
#endif

		return path;
	}

	size_t Path::calc_hash(const WStringView str)
	{
		static constexpr HashFunc<WString> hash_func;
		return !str.empty() ? hash_func(str.data()) : 0;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Path&)
	{
		return builder << '\'' << value.data() << '\'';
	}
}