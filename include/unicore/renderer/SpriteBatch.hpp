#pragma once
#include "unicore/math/Rect.hpp"
#include "unicore/math/Transform2.hpp"
#include "unicore/system/TextBlock.hpp"
#include "unicore/renderer/Vertex.hpp"
#include "unicore/renderer/sdl2/PipelineRender.hpp"
#include "unicore/renderer/ogl1/Geometry.hpp"

namespace unicore
{
	class Texture;
	class Sprite;
	class Font;

	enum class SpriteBatchEffect
	{
		None,
		FlipHorizontal,
		FlipVertical,
		FlipBoth,
	};

	class SpriteBatch : public Object
	{
		UC_OBJECT(SpriteBatch, Object)
	public:
		SpriteBatch() = default;

		void render(sdl2::PipelineRender& renderer) const;
		void render(ogl1::Geometry& renderer) const;

		SpriteBatch& clear();
		SpriteBatch& flush();

		// TRIANGLE
		SpriteBatch& draw_tri(
			const VertexColorTexture2f& v0, const VertexColorTexture2f& v1, const VertexColorTexture2f& v2,
			const Shared<Texture>& texture = nullptr);

		SpriteBatch& draw_tri(const VertexColorTexture2f* arr,
			const Shared<Texture>& texture = nullptr);

		// QUAD
		SpriteBatch& draw_quad(
			const VertexColorTexture2f& v0, const VertexColorTexture2f& v1,
			const VertexColorTexture2f& v2, const VertexColorTexture2f& v3,
			const Shared<Texture>& texture = nullptr);

		SpriteBatch& draw_quad(const VertexColorTexture2f* arr,
			const Shared<Texture>& texture = nullptr);

		// RECT
		SpriteBatch& draw(
			const Rectf& rect, const Color4b& color = ColorConst4b::White,
			const Shared<Texture>& texture = nullptr,
			const Optional<Rectf>& uv = std::nullopt);

		// TEXTURE
		SpriteBatch& draw(const Shared<Texture>& texture, const Vector2f& center,
			const Color4b& color = ColorConst4b::White,
			SpriteBatchEffect effect = SpriteBatchEffect::None);

		SpriteBatch& draw(const Shared<Texture>& texture, const Vector2f& center,
			const Recti& part, const Color4b& color = ColorConst4b::White,
			SpriteBatchEffect effect = SpriteBatchEffect::None);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Radians& angle, const Vector2f& scale,
			const Color4b& color = ColorConst4b::White,
			SpriteBatchEffect effect = SpriteBatchEffect::None);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Recti& part, const Radians& angle,
			const Vector2f& scale, const Color4b& color = ColorConst4b::White,
			SpriteBatchEffect effect = SpriteBatchEffect::None);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Transform2f& tr,
			const Color4b& color = ColorConst4b::White,
			SpriteBatchEffect effect = SpriteBatchEffect::None);

		SpriteBatch& draw(const Shared<Texture>& texture,
			const Vector2f& center, const Transform2f& tr, const Recti& part,
			const Color4b& color = ColorConst4b::White,
			SpriteBatchEffect effect = SpriteBatchEffect::None);

		// SPRITE
		SpriteBatch& draw(const Shared<Sprite>& sprite, const Vector2f& center,
			const Color4b& color = ColorConst4b::White,
			SpriteBatchEffect effect = SpriteBatchEffect::None);

		SpriteBatch& draw(const Shared<Sprite>& sprite, const Vector2f& center,
			const Radians& angle, const Vector2f& scale,
			const Color4b& color = ColorConst4b::White,
			SpriteBatchEffect effect = SpriteBatchEffect::None);

		// FONT
		SpriteBatch& print(const Shared<Font>& font, const Vector2f& pos,
			StringView32 text, const Color4b& color = ColorConst4b::White);

		SpriteBatch& print(const Shared<Font>& font, const Transform2f& tr,
			StringView32 text, const Color4b& color = ColorConst4b::White);

		SpriteBatch& print(const TextBlock& block, const Vector2f& pos,
			TextAlign align = TextAlign::TopLeft, const Color4b& color = ColorConst4b::White);

		SpriteBatch& print(const AlignedTextBlock& block, const Vector2f& pos,
			const Color4b& color = ColorConst4b::White);

	protected:
		struct Batch
		{
			Shared<Texture> texture;
			UInt32 start = 0;
			UInt32 count = 0;
		};

		List<VertexColorTexture2f> _vertices;
		List<Batch> _batches;
		Batch _current;

		void set_texture(const Shared<Texture>& texture);

		static void calc_quad_position(const Vector2f& center, const Vector2i& size,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);

		static void calc_quad_position(const Vector2f& center, const Vector2i& size,
			const Radians& angle, const Vector2f& scale,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);

		static void calc_quad_position(
			const Vector2f& center, const Vector2i& size, const Transform2f& tr,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);

		static void calc_quad_uv(SpriteBatchEffect effect,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);

		static void calc_quad_uv(
			const Vector2i& size, const Recti& rect, SpriteBatchEffect effect,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);
	};
}