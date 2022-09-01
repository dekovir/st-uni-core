#include "WinFileProvider.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include <Shlwapi.h>
#include "WinError.hpp"
#include "WinFile.hpp"

namespace unicore
{
	WinFileProvider::WinFileProvider(Logger& logger)
		: _logger(logger)
	{
	}

	bool WinFileProvider::exists(const Path& path) const
	{
		const auto native_path = path.native_path();
		const auto flags = GetFileAttributesW(native_path.c_str());
		return flags != INVALID_FILE_ATTRIBUTES ? true : false;
	}

	Optional<FileStats> WinFileProvider::stats(const Path& path) const
	{
		const auto native_path = path.native_path();

		// TODO: Test for directory
		WIN32_FILE_ATTRIBUTE_DATA fileAttr;
		if (GetFileAttributesExW(native_path.c_str(),
			GetFileExInfoStandard, &fileAttr) != 0)
		{
			SYSTEMTIME st;
			FileTimeToSystemTime(&fileAttr.ftCreationTime, &st);

			FileStats stats;
			stats.accestime = to_datetime(fileAttr.ftLastAccessTime);
			stats.createtime = to_datetime(fileAttr.ftCreationTime);
			stats.modtime = to_datetime(fileAttr.ftLastWriteTime);
			if (fileAttr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				stats.flag = FileFlag::Directory;
			else
				stats.flag = FileFlag::File;

			return stats;
		}

		if (const auto error = WinError::get_last(); !error.is_not_found())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";

		return std::nullopt;
	}

	uint16_t WinFileProvider::enumerate(const Path& path,
		WStringView search_pattern, List<Path>& name_list, FileFlags flags) const
	{
		const auto native_path = (path / search_pattern).native_path();

		uint16_t count = 0;

		// TODO: Test for directories
		WIN32_FIND_DATAW data;
		const auto find = FindFirstFileW(native_path.c_str(), &data);
		if (find != INVALID_HANDLE_VALUE)
		{
			do
			{
				const auto fileName = std::wstring(data.cFileName);
				if (fileName == L"." || fileName == L"..")
					continue;

				const FileFlag fileType = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0
					? FileFlag::Directory : FileFlag::File;
				if (!flags.has(fileType))
					continue;

				name_list.push_back(path / fileName);
				count++;
			} while (FindNextFileW(find, &data));

			FindClose(find);
		}

		return count;
	}

	bool WinFileProvider::create_directory(const Path& path)
	{
		const auto native_path = path.native_path();
		if (CreateDirectoryW(native_path.data(), nullptr) != 0)
			return true;

		if (const auto error = WinError::get_last(); error.is_error())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";
		return false;
	}

	bool WinFileProvider::delete_directory(const Path& path, bool recursive)
	{
		const auto native_path = path.native_path();
		if (RemoveDirectoryW(native_path.data()) != 0)
			return true;

		if (const auto error = WinError::get_last(); error.is_error())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";
		return false;
	}

	Shared<ReadFile> WinFileProvider::open_read(const Path& path)
	{
		const auto native_path = path.native_path();

		auto handle = CreateFileW(native_path.c_str(),
			GENERIC_READ, FILE_SHARE_READ, nullptr,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle != INVALID_HANDLE_VALUE)
			return std::make_shared<WinFile>(handle);

		if (const auto error = WinError::get_last(); !error.is_not_found())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";

		return nullptr;
	}

	Shared<WriteFile> WinFileProvider::create_new(const Path& path)
	{
		const auto native_path = path.native_path();

		auto handle = CreateFileW(native_path.c_str(),
			GENERIC_WRITE, 0, nullptr,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle != INVALID_HANDLE_VALUE)
			return std::make_shared<WinFile>(handle);

		if (const auto error = WinError::get_last(); !error.is_not_found())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";

		return nullptr;
	}

	bool WinFileProvider::delete_file(const Path& path)
	{
		const auto native_path = path.native_path();
		if (DeleteFileW(native_path.data()) != 0)
			return true;

		if (const auto error = WinError::get_last(); error.is_error())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";
		return false;
	}

	DateTime WinFileProvider::to_datetime(FILETIME const& ft)
	{
		ULARGE_INTEGER ull;
		ull.LowPart = ft.dwLowDateTime;
		ull.HighPart = ft.dwHighDateTime;
		return DateTime(ull.QuadPart / 10000000ULL - 11644473600ULL);
	}
}
#endif