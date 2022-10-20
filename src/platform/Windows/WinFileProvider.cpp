#include "WinFileProvider.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include <Shlwapi.h>
#include "WinError.hpp"
#include "WinFile.hpp"

namespace unicore
{
	static wchar_t s_buffer[MAX_PATH];

	WinFileProvider::WinFileProvider(Logger& logger)
		: _logger(logger), _current_dir(get_current_dir())
	{
	}

	WinFileProvider::WinFileProvider(Logger& logger, const Path& current_dir)
		: _logger(logger), _current_dir(current_dir)
	{
	}

	bool WinFileProvider::exists(const Path& path) const
	{
		const auto native_path = to_native_path(path);
		const auto flags = GetFileAttributesW(native_path.c_str());
		return flags != INVALID_FILE_ATTRIBUTES ? true : false;
	}

	Optional<FileStats> WinFileProvider::stats(const Path& path) const
	{
		const auto native_path = to_native_path(path);

		// TODO: Test for directory
		WIN32_FILE_ATTRIBUTE_DATA fileAttr;
		if (GetFileAttributesExW(native_path.c_str(),
			GetFileExInfoStandard, &fileAttr) != 0)
		{
			SYSTEMTIME st;
			FileTimeToSystemTime(&fileAttr.ftCreationTime, &st);

			FileStats stats;
			stats.access_time = to_datetime(fileAttr.ftLastAccessTime);
			stats.create_time = to_datetime(fileAttr.ftCreationTime);
			stats.mod_time = to_datetime(fileAttr.ftLastWriteTime);
			stats.type = get_file_type(fileAttr.dwFileAttributes);

			return stats;
		}

		if (const auto error = WinError::get_last(); !error.is_not_found())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";

		return std::nullopt;
	}

	uint16_t WinFileProvider::enumerate_entries(
		const Path& path, StringView search_pattern,
		List<String>& name_list, const EnumerateOptions& options) const
	{
		const auto native_path = to_native_path(path / search_pattern);

		uint16_t count = 0;

		// TODO: Test for directories
		WIN32_FIND_DATAW data;
		const auto find = FindFirstFileW(native_path.c_str(), &data);
		if (find != INVALID_HANDLE_VALUE)
		{
			do
			{
				const auto file_name = std::wstring_view(data.cFileName);
				if (file_name == L"." || file_name == L"..")
					continue;

				const auto type = get_file_type(data.dwFileAttributes);
				if (!enumerate_test_options(type, options))
					continue;

				name_list.push_back(Unicode::to_utf8(file_name));
				count++;
			} while (FindNextFileW(find, &data));

			FindClose(find);
		}

		return count;
	}

	bool WinFileProvider::create_directory(const Path& path)
	{
		const auto native_path = to_native_path(path);
		if (CreateDirectoryW(native_path.data(), nullptr) != 0)
			return true;

		if (const auto error = WinError::get_last(); error.is_error())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";
		return false;
	}

	bool WinFileProvider::delete_directory(const Path& path, bool recursive)
	{
		const auto native_path = to_native_path(path);
		if (RemoveDirectoryW(native_path.data()) != 0)
			return true;

		if (const auto error = WinError::get_last(); error.is_error())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";
		return false;
	}

	Shared<ReadFile> WinFileProvider::open_read(const Path& path)
	{
		const auto native_path = to_native_path(path);

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
		const auto native_path = to_native_path(path);

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
		const auto native_path = to_native_path(path);
		if (DeleteFileW(native_path.data()) != 0)
			return true;

		if (const auto error = WinError::get_last(); error.is_error())
			UC_LOG_ERROR(_logger) << error << L" for '" << path << L"'";
		return false;
	}

	Path WinFileProvider::get_current_dir() const
	{
		const auto count = GetCurrentDirectoryW(MAX_PATH, s_buffer);
		if (count > 0)
			return Path(Unicode::to_utf8(std::wstring_view(s_buffer, count)));

		UC_LOG_ERROR(_logger) << "get_current_dir failed with error - " << WinError::get_last();
		return Path::Empty;
	}

	std::wstring WinFileProvider::to_native_path(const Path& path) const
	{
		return Unicode::to_wcs((_current_dir / path).native_path());
	}

	DateTime WinFileProvider::to_datetime(FILETIME const& ft)
	{
		ULARGE_INTEGER ull;
		ull.LowPart = ft.dwLowDateTime;
		ull.HighPart = ft.dwHighDateTime;
		return DateTime(ull.QuadPart / 10000000ULL - 11644473600ULL);
	}

	FileType WinFileProvider::get_file_type(DWORD attributes)
	{
		return attributes & FILE_ATTRIBUTE_DIRECTORY
			? FileType::Directory : FileType::File;
	}
}
#endif