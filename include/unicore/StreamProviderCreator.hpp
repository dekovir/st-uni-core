#pragma once
#include "unicore/StreamProvider.hpp"

namespace unicore
{
	class StreamProviderCreator : public Object
	{
		UC_OBJECT(StreamProviderCreator, Object)
	public:
		UC_NODISCARD virtual bool can_create(const Path& path) const = 0;
		virtual Shared<ReadStreamProvider> create(const Shared<ReadStream>& stream) = 0;
	};
}