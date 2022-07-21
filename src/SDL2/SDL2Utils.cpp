#include "SDL2Utils.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	struct StreamHandle
	{
		Shared<BasicStream> shared_link;
		BasicStream* basic = nullptr;
		ReadStream* read = nullptr;
		WriteStream* write = nullptr;

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

	SDL_RWops* SDL2Utils::from_stream(ReadStream& stream)
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

	SDL_RWops* SDL2Utils::from_stream(const Shared<ReadStream>& stream)
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

	SDL_RWops* SDL2Utils::from_stream(const Shared<WriteStream>& stream)
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

	LogHelper& operator<<(LogHelper& helper, const SDL_version& value)
	{
		return helper << value.major << L"." << value.minor << L"." << value.patch;
	}

	LogHelper& operator<<(LogHelper& helper, const SDL_DisplayOrientation& value)
	{
		switch (value)
		{
		case SDL_ORIENTATION_UNKNOWN: return helper << "Unknown";
		case SDL_ORIENTATION_LANDSCAPE: return helper << "Landscape";
		case SDL_ORIENTATION_LANDSCAPE_FLIPPED: helper << "LandscapeFlipped";
		case SDL_ORIENTATION_PORTRAIT: return helper << "Portrait";
		case SDL_ORIENTATION_PORTRAIT_FLIPPED: return helper << "PortraitFlipped";
		}
	}

	LogHelper& operator<<(LogHelper& helper, const SDL_Point& value)
	{
		return helper << value.x << "x" << value.y;
	}

	LogHelper& operator<<(LogHelper& helper, const SDL_FPoint& value)
	{
		return helper << value.x << "x" << value.y;
	}

	LogHelper& operator<<(LogHelper& helper, const SDL_Rect& value)
	{
		return helper << "[" << value.x << "," << value.y << ","
			<< value.w << "," << value.h << "]";
	}

	LogHelper& operator<<(LogHelper& helper, const SDL_FRect& value)
	{
		return helper << "[" << value.x << "," << value.y << ","
			<< value.w << "," << value.h << "]";
	}
}

#endif