#include "SolidSizeCreator.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Renderer.hpp"

namespace unicore
{
	static void fill_pixel_data(void* data, const Vector2i& size, const Color4b& color)
	{
		const auto c = color.to_format(pixel_format_abgr);
		const auto pixels = static_cast<uint32_t*>(data);

		const int total = size.area();
		for (int i = 0; i < total; i++)
			pixels[i] = c;
	}

	Shared<Surface> SurfaceSizeSurfaceCreator::create_from_data(
		const Options& context,
		const CreateResource::SolidSize& data)
	{
		MemoryChunk chunk(data.size.area() * 4);
		fill_pixel_data(chunk.data(), data.size, data.color);
		return std::make_shared<Surface>(data.size, chunk);
	}

	Shared<DynamicSurface> DynamicSurfaceSolidSizeCreator::create_from_data(
		const Options& context,
		const CreateResource::SolidSize& data)
	{
		auto surface = std::make_shared<DynamicSurface>(data.size);
		Canvas canvas(*surface);
		canvas.fill(data.color);
		return surface;
	}

	TextureSolidSizeCreator::TextureSolidSizeCreator(Renderer& renderer)
		: _renderer(renderer)
	{
	}

	Shared<Texture> TextureSolidSizeCreator::create_from_data(
		const Options& context,
		const CreateResource::SolidSize& data)
	{
		DynamicSurface surface(data.size);
		Canvas canvas(surface);
		canvas.fill(data.color);
		return _renderer.create_texture(surface);
	}
}