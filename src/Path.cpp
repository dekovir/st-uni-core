#include "unicore/Path.hpp"
#include <cwctype>
#include <algorithm>
#if defined (UNICORE_PLATFORM_WINDOWS)
#include <Shlwapi.h>
#endif
#include "unicore/Strings.hpp"
#include "unicore/LogHelper.hpp"

namespace unicore
{
	const Path Path::Empty;

	const wchar_t Path::DirSeparator = L'/';

#if defined (ENG_PLATFORM_WIN)
	const wchar_t Path::NativeDirSeparator = L'\\';
#else
	const wchar_t Path::NativeDirSeparator = L'/';
#endif

	static const wchar_t WrongDirSeparator = L'\\';
	static const wchar_t DriveSeparator = L':';

	Path::Path(const WStringView path)
		: _path(prepare(path))
	{
		_hash = calc_hash(_path);
	}

	Path::Path(const WString& path, std::size_t hash)
		: _path(path), _hash(hash)
	{
	}

	bool Path::absolute() const
	{
		return find_drive_pos(_path) != WString::npos;
	}

	void Path::clear()
	{
		_path.clear();
		_hash = 0;
	}

	bool Path::has_extension() const
	{
		const auto pos = find_extension_pos(_path);
		return pos != WString::npos;
	}

	void Path::parent_path(Path& parentPath) const
	{
		const auto pos = find_filename_pos(_path);
		if (pos != WString::npos)
			parentPath = Path(_path.substr(0, pos));
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
		const auto pos = find_filename_pos(_path);
		if (pos != WString::npos)
			fileName = _path.substr(pos + 1);
		else fileName = _path;
	}

	WString Path::filename() const
	{
		WString str;
		filename(str);
		return str;
	}

	void Path::extension(WString& ext) const
	{
		const auto pos = find_extension_pos(_path);
		if (pos != std::u32string::npos)
			ext = _path.substr(pos);
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
		const auto pos = find_filename_pos(_path);
		if (pos != WString::npos)
		{
			parentDir = Path(_path.substr(0, pos));
			fileName = _path.substr(pos + 1);
		}
		else
		{
			parentDir = Empty;
			fileName = _path;
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
		while ((pos = _path.find(DirSeparator, prev)) != WString::npos)
		{
			const auto count = pos - prev;
			if (count > 0)
				elements.push_back(_path.substr(prev, count));
			prev = pos + 1;
		}

		elements.push_back(_path.substr(prev));
	}

	void Path::replace_filename(const WString& filename)
	{
		const auto pos = find_filename_pos(_path);
		if (pos != WString::npos)
		{
			_path = _path.substr(0, pos + 1);
			_path += filename;
		}
		else
		{
			_path = filename;
		}

		_hash = calc_hash(_path);
	}

	void Path::replace_extension(const WString& ext)
	{
		const auto pos = find_extension_pos(_path);
		if (pos != WString::npos)
			_path = _path.substr(0, pos);
		else if (ext.empty())
			return;

		_path += ext;
		_hash = calc_hash(_path);
	}

	void Path::remove_extension(const List<WString>& extensions)
	{
		for (const auto& ext : extensions)
		{
			if (!Strings::ends_with(_path, ext, true))
				continue;

			_path = _path.substr(0, _path.size() - ext.length());
			_hash = calc_hash(_path);
			break;
		}
	}

	bool Path::equals(const Path& other) const
	{
		return _hash == other._hash ||
			Strings::compare(_path, other._path, false) == 0;
	}

	int Path::compare(const Path& other) const
	{
		if (_hash < other._hash)
			return -1;
		if (_hash > other._hash)
			return +1;

		return Strings::compare(_path, other._path, false);
	}

	Path Path::operator/(const WString& file) const
	{
		if (file.empty())
			return *this;

		if (empty())
			return Path(file);

		return Path(_path + DirSeparator + file);
	}

	Path Path::operator/(const Path& path) const
	{
		if (path.empty())
			return *this;

		if (empty())
			return path;

		return Path(_path + DirSeparator + path._path);
	}

	void Path::native_path(WString& nativePath) const
	{
		nativePath = _path;
		if (DirSeparator != NativeDirSeparator)
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
		return Path(_path, _hash);
#else
		auto tmp = _path;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), towlower);
		return Path(tmp);
#endif
	}

	Path Path::combine(const WString& a, const WString& b)
	{
		return Path(a + DirSeparator + b);
	}

	Path Path::combine(const WString& a, const WString& b, const WString& c)
	{
		return Path(a + DirSeparator + b + DirSeparator + c);
	}

	// =========================================================================
	WString::size_type Path::find_drive_pos(const WString& str)
	{
		return str.find_last_of(DriveSeparator);
	}

	WString::size_type Path::find_filename_pos(const WString& str)
	{
		return str.find_last_of(DirSeparator);
	}

	WString::size_type Path::find_extension_pos(const WString& str)
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

	size_t Path::calc_hash(const WString& str)
	{
		static const HashFunc<WString> hash_func;
		return !str.empty() ? hash_func(str) : 0;
	}

	LogHelper& operator<<(LogHelper& helper, const Path& path)
	{
		return helper << path.data();
	}
}