#pragma once
#include "unicore/Color4.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Angle.hpp"
#include "unicore/FileSystem.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"
#include "unicore/RenderState.hpp"
#include "unicore/Vertex.hpp"

namespace unicore
{
	class Render : public Module
	{
	public:
		virtual const Vector2i& screen_size() const = 0;
		virtual const RenderState& state() const = 0;

		virtual void set_state(const RenderState& state) = 0;

		virtual Shared<Surface> load_surface(const Shared<ReadStream>& stream) = 0;
		virtual Shared<Texture> create_texture(Surface& surface) = 0;

		virtual Shared<Texture> load_texture(const Shared<ReadStream>& stream);

		virtual bool begin_scene() = 0;
		virtual void end_scene() = 0;

		virtual void clear(const Color4b& color) = 0;

		virtual void draw_geometry(
			const Vertex* vertices, size_t num_vertices
		) = 0;
	};
}