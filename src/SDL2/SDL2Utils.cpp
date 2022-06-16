#include "SDL2Utils.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	struct StreamHandle
	{
		BasicStream* basic;
		Shared<ReadStream> read;
		Shared<WriteStream> write;
	};

	static Sint64 basic_size_func(SDL_RWops* context)
	{
		auto handle = static_cast<StreamHandle*>(context->hidden.unknown.data1);
		return handle->basic->size();
	}

	static Sint64 basic_seek_func(SDL_RWops* context, Sint64 offset, int whence)
	{
		auto handle = static_cast<StreamHandle*>(context->hidden.unknown.data1);
		return handle->basic->seek(offset, static_cast<SeekMethod>(whence));
	}

	static size_t read_func(SDL_RWops* context, void* ptr, size_t size, size_t maxnum)
	{
		auto handle = static_cast<StreamHandle*>(context->hidden.unknown.data1);
		return handle->read->read(ptr, size, maxnum);
	}

	static size_t write_func(SDL_RWops* context, const void* ptr, size_t size, size_t num)
	{
		auto handle = static_cast<StreamHandle*>(context->hidden.unknown.data1);
		return handle->write->write(ptr, size, num);
	}

	static int close_func(SDL_RWops* context)
	{
		auto handle = static_cast<StreamHandle*>(context->hidden.unknown.data1);
		handle->read = nullptr;
		handle->write = nullptr;
		delete handle;
		context->hidden.unknown.data1 = nullptr;

		return 0;
	}

	SDL_RWops* SDL2Utils::from_stream(const Shared<ReadStream>& stream)
	{
		SDL_RWops* c = SDL_AllocRW();
		if (c != nullptr)
		{
			auto handle = new StreamHandle;
			handle->read = stream;
			handle->basic = stream.get();

			c->size = &basic_size_func;
			c->seek = &basic_seek_func;
			c->read = &read_func;
			c->write = &write_func;
			c->close = &close_func;
			c->type = 0xdeadbeef;
			c->hidden.unknown.data1 = handle;
		}

		return c;
	}

	SDL_RWops* SDL2Utils::from_stream(const Shared<WriteStream>& stream)
	{
		SDL_RWops* c = SDL_AllocRW();
		if (c != nullptr)
		{
			auto handle = new StreamHandle;
			handle->write = stream;
			handle->basic = stream.get();

			c->size = &basic_size_func;
			c->seek = &basic_seek_func;
			c->read = &read_func;
			c->write = &write_func;
			c->close = &close_func;
			c->type = 0;
			c->hidden.unknown.data1 = handle;
		}

		return c;
	}

	LogHelper& operator<<(LogHelper& helper, const SDL_version& item)
	{
		return helper << item.major << L"." << item.minor << L"." << item.patch;
	}
}

#endif
