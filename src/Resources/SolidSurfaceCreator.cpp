#include "SolidSurfaceCreator.hpp"

namespace unicore
{
	Shared<Surface> SurfaceSolidSurfaceCreator::create_from_data(
		const ResourceCreatorContext& context,
		const CreateResource::SolidColor& data)
	{
		const size_t total = data.size.area() * 4;

		MemoryChunk chunk(total);
		
		const auto color = data.color.to_format(pixel_format_abgr);
		const auto pixels = static_cast<uint32_t*>(chunk.data());

		for (int i = 0; i < data.size.area(); i++)
			pixels[i] = color;

		return std::make_shared<Surface>(data.size, chunk);
	}

	Shared<DynamicSurface> DynamicSurfaceSolidSurfaceCreator::create_from_data(
		const ResourceCreatorContext& context,
		const CreateResource::SolidColor& data)
	{
		auto surface = std::make_shared<DynamicSurface>(data.size);
		const auto color = data.color.to_format(pixel_format_abgr);
		const auto pixels = static_cast<uint32_t*>(surface->data());

		for (int i = 0; i < data.size.area(); i++)
			pixels[i] = color;

		return surface;
	}
}