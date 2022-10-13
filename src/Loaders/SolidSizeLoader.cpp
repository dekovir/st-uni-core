#include "unicore/loaders/SolidSizeLoader.hpp"
#include "unicore/renderer/Canvas.hpp"
#include "unicore/renderer/Surface.hpp"
#include "unicore/renderer/Renderer.hpp"

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

	// SurfaceSizeSurfaceLoader ///////////////////////////////////////////////////
	Shared<Resource> SurfaceSizeSurfaceLoader::load_options(
		const Context& context, const SolidSizeOptions& options)
	{
		MemoryChunk chunk(options.size.area() * 4);
		fill_pixel_data(chunk.data(), options.size, options.color);
		return std::make_shared<Surface>(options.size, std::move(chunk));
	}

	// DynamicSurfaceSolidSizeLoader //////////////////////////////////////////////
	Shared<Resource> DynamicSurfaceSolidSizeLoader::load_options(
		const Context& context, const SolidSizeOptions& options)
	{
		auto surface = std::make_shared<DynamicSurface>(options.size);
		Canvas canvas(*surface);
		canvas.fill(options.color);
		return surface;
	}

	// TextureSolidSizeLoader /////////////////////////////////////////////////////
	TextureSolidSizeLoader::TextureSolidSizeLoader(Renderer& renderer)
		: _renderer(renderer)
	{
	}

	Shared<Resource> TextureSolidSizeLoader::load_options(
		const Context& context, const SolidSizeOptions& options)
	{
		DynamicSurface surface(options.size);
		Canvas canvas(surface);
		canvas.fill(options.color);
		return _renderer.create_texture(surface);
	}
}