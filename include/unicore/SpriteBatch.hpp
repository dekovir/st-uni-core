#pragma once
#include "unicore/Object.hpp"
#include "unicore/Render2D.hpp"

namespace unicore
{
	class SpriteBatch : public Object
	{
	public:
		SpriteBatch() = default;

		void begin();
		void end();

		void clear();
		void to_render(Render2D& render) const;

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vertex2& v0, const Vertex2& v1, const Vertex2& v2);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vertex2& v0, const Vertex2& v1, const Vertex2& v2, const Vertex2& v3);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Color4b& color = ColorConst4b::White);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Radians& angle,
			const Vector2f& scale, const Color4b& color = ColorConst4b::White);

	protected:
		struct Batch
		{
			Shared<Texture> texture;
			size_t vstart = 0;
			size_t vcount = 0;
		};

		VertexList _vertices;
		List<Batch> _batches;
		Batch _current;

		void flush();

		static void calc_quad_position(const Vector2f& center, const Vector2i& size,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);

		static void calc_quad_position(const Vector2f& center, const Vector2i& size,
			const Radians& angle, const Vector2f& scale,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);
	};
}
