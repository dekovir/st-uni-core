#include "SDL2Utils.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	struct StreamHandle
	{
		Shared<File> shared_link;
		File* basic = nullptr;
		ReadFile* read = nullptr;
		WriteFile* write = nullptr;

		static StreamHandle* from_context(SDL_RWops* context)
		{
			return static_cast<StreamHandle*>(context->hidden.unknown.data1);
		}
	};

	static Sint64 basic_size_func(SDL_RWops* context)
	{
		const auto handle = StreamHandle::from_context(context);
		return handle->basic->size();
	}

	static Sint64 basic_seek_func(SDL_RWops* context, Sint64 offset, int whence)
	{
		const auto handle = StreamHandle::from_context(context);
		return handle->basic->seek(offset, static_cast<SeekMethod>(whence));
	}

	static size_t read_func(SDL_RWops* context, void* ptr, size_t size, size_t maxnum)
	{
		size_t count;
		const auto handle = StreamHandle::from_context(context);
		handle->read->read(ptr, size * maxnum, &count);
		return count / size;
	}

	static size_t write_func(SDL_RWops* context, const void* ptr, size_t size, size_t num)
	{
		size_t count;
		const auto handle = StreamHandle::from_context(context);
		handle->write->write(ptr, size * num, &count);
		return count / size;
	}

	static int close_func(SDL_RWops* context)
	{
		if (const auto handle = StreamHandle::from_context(context))
		{
			handle->shared_link = nullptr;
			handle->basic = nullptr;
			handle->read = nullptr;
			handle->write = nullptr;
			delete handle;
			context->hidden.unknown.data1 = nullptr;
		}

		return 0;
	}

	static void init_context(SDL_RWops* context, StreamHandle* handle)
	{
		context->size = &basic_size_func;
		context->seek = &basic_seek_func;
		context->read = &read_func;
		context->write = &write_func;
		context->close = &close_func;
		context->type = 0;
		context->hidden.unknown.data1 = handle;
	}

	bool SDL2Utils::get_desktop_native_mode(SDL_DisplayMode& mode)
	{
		return SDL_GetDesktopDisplayMode(0, &mode) == 0;
	}

	SDL_RWops* SDL2Utils::from_stream(ReadFile& stream)
	{
		SDL_RWops* c = SDL_AllocRW();
		if (c != nullptr)
		{
			const auto handle = new StreamHandle;
			handle->basic = &stream;
			handle->read = &stream;
			init_context(c, handle);
		}

		return c;
	}

	SDL_RWops* SDL2Utils::from_stream(const Shared<ReadFile>& stream)
	{
		SDL_RWops* c = SDL_AllocRW();
		if (c != nullptr)
		{
			const auto handle = new StreamHandle;
			handle->shared_link = stream;
			handle->basic = stream.get();
			handle->read = stream.get();
			init_context(c, handle);
		}

		return c;
	}

	SDL_RWops* SDL2Utils::from_stream(const Shared<WriteFile>& stream)
	{
		SDL_RWops* c = SDL_AllocRW();
		if (c != nullptr)
		{
			const auto handle = new StreamHandle;
			handle->shared_link = stream;
			handle->basic = stream.get();
			handle->write = stream.get();
			init_context(c, handle);
		}

		return c;
	}

	UNICODE_STRING_BUILDER_FORMAT(const SDL_version&)
	{
		return builder << value.major << L"." << value.minor << L"." << value.patch;
	}

	UNICODE_STRING_BUILDER_FORMAT(const SDL_DisplayOrientation&)
	{
		switch (value)
		{
		case SDL_ORIENTATION_UNKNOWN: return builder << "Unknown";
		case SDL_ORIENTATION_LANDSCAPE: return builder << "Landscape";
		case SDL_ORIENTATION_LANDSCAPE_FLIPPED: builder << "LandscapeFlipped";
		case SDL_ORIENTATION_PORTRAIT: return builder << "Portrait";
		case SDL_ORIENTATION_PORTRAIT_FLIPPED: return builder << "PortraitFlipped";
		}
		UC_ASSERT_ALWAYS_MSG("Invalid type");
		return builder;
	}

	UNICODE_STRING_BUILDER_FORMAT(const SDL_Point&)
	{
		return builder << value.x << "x" << value.y;
	}

	UNICODE_STRING_BUILDER_FORMAT(const SDL_FPoint&)
	{
		return builder << value.x << "x" << value.y;
	}

	UNICODE_STRING_BUILDER_FORMAT(const SDL_Rect&)
	{
		return builder << "[" << value.x << "," << value.y << ","
			<< value.w << "," << value.h << "]";
	}

	UNICODE_STRING_BUILDER_FORMAT(const SDL_FRect&)
	{
		return builder << "[" << value.x << "," << value.y << ","
			<< value.w << "," << value.h << "]";
	}
}

#endif