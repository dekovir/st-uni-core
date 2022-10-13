#pragma once
#include "unicore/resource/Resource.hpp"

namespace unicore
{
	class Renderer;

	class RendererResource : public Resource
	{
		UC_OBJECT(RendererResource, Resource)
	public:
		UC_NODISCARD virtual size_t get_video_memory_use() const = 0;
		//UC_NODISCARD virtual Render& render() const = 0;
	};
}