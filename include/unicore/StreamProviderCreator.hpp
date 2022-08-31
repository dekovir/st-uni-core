#pragma once
#include "unicore/StreamProvider.hpp"

namespace unicore
{
	class StreamProviderCreator : public Object
	{
		UC_OBJECT(StreamProviderCreator, Object)
	public:
		struct Options
		{
			Any value;
			ReadStreamProvider& provider;
			Logger* logger = nullptr;
		};

		UC_NODISCARD virtual bool can_create(const Options& options) const = 0;
		virtual Shared<ReadStreamProvider> create(const Options& options) = 0;
	};
}