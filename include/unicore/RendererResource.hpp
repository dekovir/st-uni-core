#pragma once
#include "unicore/Resource.hpp"

namespace unicore
{
	class Renderer;

	class RendererResource : public Resource
	{
		UC_OBJECT(RendererResource, Resource)
	public:
		UC_NODISCARD virtual size_t video_memory_use() const = 0;
		//UC_NODISCARD virtual Render& render() const = 0;
	};
}