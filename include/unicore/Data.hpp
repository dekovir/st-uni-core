#pragma once
#include "unicore/Resource.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/Stream.hpp"
#include "unicore/LogHelper.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	//class ReadStreamData : public Resource
	//{
	//public:
	//	explicit ReadStreamData(const Shared<ReadStream>& data)
	//		: _data(data) {}
	//protected:
	//	Shared<ReadStream> _data;
	//};

	class BinaryData : public Resource
	{
	public:
		BinaryData(void* buffer, size_t size, bool free_buffer = true)
			: _buffer(buffer), _size(size), _free_buffer(free_buffer) {}

		~BinaryData() override
		{
			if (_free_buffer)
				Memory::free(_buffer);
		}

		UC_NODISCARD const void* data() const { return _buffer; }
		UC_NODISCARD size_t size() const { return _size; }

	protected:
		void* _buffer;
		size_t _size;
		bool _free_buffer;
	};

	class BinaryDataLoader : public ResourceLoaderT<BinaryData>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override
		{
			const auto size = static_cast<size_t>(context.stream.size());
			context.stream.seek(0);
			auto buffer = Memory::alloc(size);
			const auto readed = context.stream.read(buffer, 1, size);
			if (readed == size)
				return make_shared<BinaryData>(buffer, size);

			free(buffer);
			UC_LOG_ERROR(context.logger) << "Read failed";
			return nullptr;
		}
	};

	template<typename Char>
	class BasicStringData : public Resource
	{
	public:
		explicit BasicStringData(BasicStringView<Char> data)
			: _data(data) {}

		explicit BasicStringData(BasicString<Char>&& data)
			: _data(std::move(data)) {}

		UC_NODISCARD const BasicString<Char>& data() const { return _data; }

	protected:
		BasicString<Char> _data;
	};

	class StringData : public BasicStringData<char>
	{
	public:
		explicit StringData(StringView data)
			: BasicStringData<char>(data)
		{
		}

		explicit StringData(String&& data)
			: BasicStringData<char>(data)
		{
		}
	};

	class StringDataLoader : public ResourceLoaderT<StringData>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override
		{
			const auto size = static_cast<size_t>(context.stream.size());
			context.stream.seek(0);
			String str;
			str.resize(size);
			context.stream.read(str.data(), 1, size);
			return make_shared<StringData>(str.data());
		}
	};

	class WStringData : public BasicStringData<wchar_t>
	{
	public:
		explicit WStringData(WStringView data)
			: BasicStringData<wchar_t>(data)
		{
		}

		explicit WStringData(WString&& data)
			: BasicStringData<wchar_t>(data)
		{
		}
	};
}