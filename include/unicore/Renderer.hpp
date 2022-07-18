#pragma once
#include "unicore/Module.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Rect.hpp"
#include "unicore/RenderResource.hpp"

namespace unicore
{
	class Logger;
	class Surface;
	class Texture;

	class Renderer : public Module
	{
		UC_OBJECT(Renderer, Module)
	public:
		explicit Renderer(Logger& logger);

		UC_NODISCARD virtual const Vector2i& screen_size() const = 0;
		UC_NODISCARD virtual uint32_t draw_calls() const = 0;

		virtual Shared<Texture> create_texture(Surface& surface) = 0;
		virtual bool update_texture(Texture& texture, Surface& surface,
			Optional<Recti> rect = std::nullopt) = 0;

		virtual bool begin_scene() = 0;
		virtual void end_scene() = 0;

		void register_module(Context& context) override;
		void unregister_module(Context& context) override;

	protected:
		Logger& _logger;
	};
}