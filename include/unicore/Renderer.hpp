#pragma once
#include "unicore/Module.hpp"
#include "unicore/math/Rect.hpp"

namespace unicore
{
	class Logger;
	class Surface;

	class Texture;
	class DynamicTexture;
	class TargetTexture;

	class Renderer : public Module
	{
		UC_OBJECT(Renderer, Module)
	public:
		explicit Renderer(Logger& logger);

		UC_NODISCARD virtual const Vector2i& screen_size() const = 0;
		UC_NODISCARD virtual uint32_t draw_calls() const = 0;

		virtual Shared<Texture> create_texture(Surface& surface) = 0;

		virtual Shared<DynamicTexture> create_dynamic_texture(const Vector2i& size) = 0;
		virtual bool update_texture(DynamicTexture& texture, Surface& surface,
			Optional<Recti> rect = std::nullopt) = 0;

		virtual Shared<TargetTexture> create_target_texture(const Vector2i& size) = 0;

		virtual bool set_target(const Shared<TargetTexture>& texture) = 0;
		UC_NODISCARD virtual const Shared<TargetTexture>& get_target() const = 0;

		virtual bool begin_frame() = 0;
		virtual void end_frame() = 0;

	protected:
		Logger& _logger;
	};
}