#pragma once
#include "unicore/Rect.hpp"
#include "unicore/Vertex.hpp"

namespace unicore
{
	class Texture;
	class Sprite;
	class Font;
	class RendererSDL;

	class SpriteBatch : public Object
	{
		UC_OBJECT(SpriteBatch, Object)
	public:
		SpriteBatch() = default;

		void render(RendererSDL& renderer) const;

		SpriteBatch& clear();
		SpriteBatch& flush();

		// TRIANGLE
		SpriteBatch& draw(
			const VertexTexColor2& v0, const VertexTexColor2& v1, const VertexTexColor2& v2,
			const Shared<Texture>& texture = nullptr);

		// QUAD
		SpriteBatch& draw(
			const VertexTexColor2& v0, const VertexTexColor2& v1,
			const VertexTexColor2& v2, const VertexTexColor2& v3,
			const Shared<Texture>& texture = nullptr);

		// RECT
		SpriteBatch& draw(
			const Rectf& rect, const Color4b& color = ColorConst4b::White,
			const Shared<Texture>& texture = nullptr,
			const Optional<Rectf>& uv = std::nullopt);

		// TEXTURE
		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Color4b& color = ColorConst4b::White);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Recti& part, const Color4b& color = ColorConst4b::White);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Radians& angle,
			const Vector2f& scale, const Color4b& color = ColorConst4b::White);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Recti& part, const Radians& angle,
			const Vector2f& scale, const Color4b& color = ColorConst4b::White);

		// SPRITE
		SpriteBatch& draw(const Shared<Sprite>& sprite,
			const Vector2f& center, const Color4b& color = ColorConst4b::White);

		SpriteBatch& draw(const Shared<Sprite>& sprite,
			const Vector2f& center, const Radians& angle,
			const Vector2f& scale, const Color4b& color = ColorConst4b::White);

		// FONT
		SpriteBatch& print(const Shared<Font>& font,
			const Vector2f& pos, StringView text,
			const Color4b& color = ColorConst4b::White);

	protected:
		struct Batch
		{
			Shared<Texture> texture;
			size_t vstart = 0;
			size_t vcount = 0;
		};

		List<VertexTexColor2> _vertices;
		List<Batch> _batches;
		Batch _current;

		void set_texture(const Shared<Texture>& texture);

		static void calc_quad_position(const Vector2f& center, const Vector2i& size,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);

		static void calc_quad_position(const Vector2f& center, const Vector2i& size,
			const Radians& angle, const Vector2f& scale,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);

		static void calc_quad_uv(const Vector2i& size, const Recti& rect,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);
	};
}