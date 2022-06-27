#pragma once
#include "unicore/Resource.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
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

		UC_NODISCARD size_t system_memory_use() const override { return sizeof(BinaryData) + _size; }

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
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};

	template<typename Char>
	class BasicTextData : public Resource
	{
	public:
		explicit BasicTextData(BasicStringView<Char> data)
			: _data(data) {}

		explicit BasicTextData(BasicString<Char>&& data)
			: _data(std::move(data)) {}

		UC_NODISCARD size_t system_memory_use() const override { return sizeof(BasicTextData<Char>) + _data.size() * sizeof(Char); }

		UC_NODISCARD const BasicString<Char>& data() const { return _data; }

	protected:
		BasicString<Char> _data;
	};

	class TextData : public BasicTextData<char>
	{
	public:
		explicit TextData(StringView data)
			: BasicTextData<char>(data)
		{
		}

		explicit TextData(String&& data)
			: BasicTextData<char>(data)
		{
		}
	};

	class TextDataLoader : public ResourceLoaderT<TextData>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};

	class WStringData : public BasicTextData<wchar_t>
	{
	public:
		explicit WStringData(WStringView data)
			: BasicTextData<wchar_t>(data)
		{
		}

		explicit WStringData(WString&& data)
			: BasicTextData<wchar_t>(data)
		{
		}
	};
}