#include "unicore/renderer/SpriteBatch.hpp"
#include "unicore/renderer/Font.hpp"
#include "unicore/renderer/Texture.hpp"
#include "unicore/renderer/Sprite.hpp"

namespace unicore
{
	static VertexColorTexture2f s_quad[4];
	static List<QuadColor2f> s_quad_list;
	static Dictionary<Shared<Texture>, List<QuadColorTexture2f>> s_quad_dict;

	static void convert(const VertexColor2f& from, VertexColorTexture2f& to)
	{
		to.pos = from.pos;
		to.col = from.col;
	}

	void SpriteBatch::render(sdl2::PipelineRender& renderer) const
	{
		for (const auto& batch : _batches)
		{
			renderer.draw_trianglesf(&_vertices[batch.start],
				batch.count, batch.texture.get());
		}
	}

	void SpriteBatch::render(ogl1::Geometry& renderer) const
	{
		renderer.begin(ogl1::RenderMode::Triangles);

		for (const auto& batch : _batches)
		{
			renderer.bind_texture(batch.texture);
			renderer.vertex_tex_color2fv(&_vertices[batch.start], batch.count);
		}

		renderer.end();
	}

	SpriteBatch& SpriteBatch::clear()
	{
		_batches.clear();
		_current = {};
		_vertices.clear();

		return *this;
	}

	SpriteBatch& SpriteBatch::flush()
	{
		if (_current.count > 0)
		{
			_batches.push_back(_current);

			_current = {};
			_current.start = static_cast<UInt16>(_vertices.size());
		}

		return *this;
	}

	// DRAW TRIANGLE /////////////////////////////////////////////////////////////
	SpriteBatch& SpriteBatch::draw_tri(
		const VertexColorTexture2f& v0, const VertexColorTexture2f& v1, const VertexColorTexture2f& v2,
		const Shared<Texture>& texture)
	{
		set_texture(texture);

		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
		_current.count += 3;

		return *this;
	}

	SpriteBatch& SpriteBatch::draw_tri(const VertexColorTexture2f* arr,
		const Shared<Texture>& texture)
	{
		set_texture(texture);

		_vertices.push_back(arr[0]);
		_vertices.push_back(arr[1]);
		_vertices.push_back(arr[2]);
		_current.count += 3;

		return *this;
	}

	// DRAW QUAD /////////////////////////////////////////////////////////////////
	SpriteBatch& SpriteBatch::draw_quad(
		const VertexColorTexture2f& v0, const VertexColorTexture2f& v1,
		const VertexColorTexture2f& v2, const VertexColorTexture2f& v3,
		const Shared<Texture>& texture)
	{
		set_texture(texture);

		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v3);

		_vertices.push_back(v3);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
		_current.count += 6;

		return *this;
	}

	SpriteBatch& SpriteBatch::draw_quad(const VertexColorTexture2f* arr,
		const Shared<Texture>& texture)
	{
		set_texture(texture);

		_vertices.push_back(arr[0]);
		_vertices.push_back(arr[1]);
		_vertices.push_back(arr[3]);

		_vertices.push_back(arr[3]);
		_vertices.push_back(arr[1]);
		_vertices.push_back(arr[2]);
		_current.count += 6;

		return *this;
	}

	// DRAW RECT //////////////////////////////////////////////////////////////////
	SpriteBatch& SpriteBatch::draw(const Rectf& rect, const Color4b& color,
		const Shared<Texture>& texture, const Optional<Rectf>& uv)
	{
		set_texture(texture);

		s_quad[0].pos = rect.bottom_left();
		s_quad[1].pos = rect.bottom_right();
		s_quad[2].pos = rect.top_right();
		s_quad[3].pos = rect.top_left();

		if (uv.has_value())
		{
			const auto r = uv.value();
			s_quad[0].uv = r.bottom_left();
			s_quad[1].uv = r.bottom_right();
			s_quad[2].uv = r.top_right();
			s_quad[3].uv = r.top_left();
		}
		else
		{
			s_quad[0].uv = Vector2f(0, 0);
			s_quad[1].uv = Vector2f(1, 0);
			s_quad[2].uv = Vector2f(1, 1);
			s_quad[3].uv = Vector2f(0, 1);
		}

		s_quad[0].col = color;
		s_quad[1].col = color;
		s_quad[2].col = color;
		s_quad[3].col = color;

		return draw_quad(s_quad, texture);
	}

	// DRAW TEXTURE ///////////////////////////////////////////////////////////////
	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const Vector2f& center, const Color4b& color, SpriteBatchEffect effect)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(),
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

			calc_quad_uv(effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const Vector2f& center, const Recti& part,
		const Color4b& color, SpriteBatchEffect effect)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(),
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);
			calc_quad_uv(texture->size(), part, effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const Vector2f& center, const Radians& angle, const Vector2f& scale,
		const Color4b& color, SpriteBatchEffect effect)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(), angle, scale,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

			calc_quad_uv(effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const Vector2f& center, const Recti& part, const Radians& angle,
		const Vector2f& scale, const Color4b& color, SpriteBatchEffect effect)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(), angle, scale,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

			calc_quad_uv(texture->size(), part, effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const Vector2f& center, const Transform2f& tr,
		const Color4b& color, SpriteBatchEffect effect)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(), tr,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

			calc_quad_uv(effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const Vector2f& center, const Transform2f& tr,
		const Recti& part, const Color4b& color, SpriteBatchEffect effect)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(), tr,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

			calc_quad_uv(texture->size(), part, effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	// DRAW SPRITE ////////////////////////////////////////////////////////////////
	SpriteBatch& SpriteBatch::draw(const Shared<Sprite>& sprite,
		const Vector2f& center, const Color4b& color, SpriteBatchEffect effect)
	{
		if (sprite)
		{
			auto& rect = sprite->rect();
			auto& texture = sprite->texture();

			calc_quad_position(center, rect.size,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);
			calc_quad_uv(texture->size(), rect, effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Sprite>& sprite,
		const Vector2f& center, const Radians& angle, const Vector2f& scale,
		const Color4b& color, SpriteBatchEffect effect)
	{
		if (sprite)
		{
			auto& rect = sprite->rect();
			auto& texture = sprite->texture();

			calc_quad_position(center, rect.size, angle, scale,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);
			calc_quad_uv(texture->size(), rect, effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Sprite>& sprite,
		const Vector2f& center, const Transform2f& tr,
		const Color4b& color, SpriteBatchEffect effect)
	{
		if (sprite)
		{
			auto& rect = sprite->rect();
			auto& texture = sprite->texture();

			calc_quad_position(center, rect.size, tr,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);
			calc_quad_uv(texture->size(), rect, effect,
				s_quad[0].uv, s_quad[1].uv, s_quad[2].uv, s_quad[3].uv);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	// DRAW FONT /////////////////////////////////////////////////////////////////
	SpriteBatch& SpriteBatch::print(const Shared<Font>& font,
		const Vector2f& pos, StringView32 text, const Color4b& color)
	{
		if (const auto textured = std::dynamic_pointer_cast<TexturedFont>(font))
		{
			s_quad_dict.clear();
			textured->generate(pos, text, color, s_quad_dict);

			for (const auto& [tex, quad_list] : s_quad_dict)
			{
				for (const auto& quad : quad_list)
					draw_quad(quad.v, tex);
			}
		}

		if (const auto geometry = std::dynamic_pointer_cast<GeometryFont>(font))
		{
			s_quad_list.clear();
			geometry->generate(pos, text, color, s_quad_list);

			for (const auto& quad : s_quad_list)
			{
				convert(quad.v[0], s_quad[0]);
				convert(quad.v[1], s_quad[1]);
				convert(quad.v[2], s_quad[2]);
				convert(quad.v[3], s_quad[3]);

				draw_quad(s_quad);
			}
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::print(const Shared<Font>& font,
		const Transform2f& tr, StringView32 text, const Color4b& color)
	{
		if (const auto textured = std::dynamic_pointer_cast<TexturedFont>(font))
		{
			s_quad_dict.clear();
			textured->generate(VectorConst2f::Zero, text, color, s_quad_dict);

			for (auto& [tex, quad_list] : s_quad_dict)
			{
				for (auto& quad : quad_list)
				{
					tr.apply(quad.v[0].pos);
					tr.apply(quad.v[1].pos);
					tr.apply(quad.v[2].pos);
					tr.apply(quad.v[3].pos);
					draw_quad(quad.v, tex);
				}
			}
		}

		if (const auto geometry = std::dynamic_pointer_cast<GeometryFont>(font))
		{
			s_quad_list.clear();
			geometry->generate(VectorConst2f::Zero, text, color, s_quad_list);

			for (const auto& quad : s_quad_list)
			{
				convert(quad.v[0], s_quad[0]);
				convert(quad.v[1], s_quad[1]);
				convert(quad.v[2], s_quad[2]);
				convert(quad.v[3], s_quad[3]);

				tr.apply(s_quad[0].pos);
				tr.apply(s_quad[1].pos);
				tr.apply(s_quad[2].pos);
				tr.apply(s_quad[3].pos);

				draw_quad(s_quad);
			}
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::print(const TextBlock& block,
		const Vector2f& pos, TextAlign align, const Color4b& color)
	{
		static List<Vector2f> offset;

		TextBlock::calc_align_offset(block.lines(), align, offset);

		for (unsigned i = 0; i < block.lines().size(); i++)
			print(block.font(), pos + offset[i], block.lines()[i].text, color);

		return *this;
	}

	SpriteBatch& SpriteBatch::print(const AlignedTextBlock& block,
		const Vector2f& pos, const Color4b& color)
	{
		for (unsigned i = 0; i < block.lines().size(); i++)
			print(block.font(), pos + block.offset_list()[i], block.lines()[i].text, color);

		return *this;
	}

	// ===========================================================================
	void SpriteBatch::set_texture(const Shared<Texture>& texture)
	{
		if (_current.texture != texture)
		{
			flush();
			_current.texture = texture;
		}
	}

	void SpriteBatch::calc_quad_position(
		const Vector2f& center, const Vector2i& size,
		Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3)
	{
		const auto hx = static_cast<float>(size.x) / 2;
		const auto hy = static_cast<float>(size.y) / 2;

		p0 = Vector2f(center.x - hx, center.y - hy);
		p1 = Vector2f(center.x + hx, center.y - hy);
		p2 = Vector2f(center.x + hx, center.y + hy);
		p3 = Vector2f(center.x - hx, center.y + hy);
	}

	static Vector2f rotate_and_scale(float x, float y,
		float a_cos, float a_sin, float scale_x, float scale_y)
	{
		return {
			a_cos * scale_x * x + a_sin * scale_y * y,
			a_cos * scale_y * y - a_sin * scale_x * x
		};
	}

	void SpriteBatch::calc_quad_position(
		const Vector2f& center, const Vector2i& size, const Radians& angle,
		const Vector2f& scale, Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3)
	{
		const auto hx = static_cast<float>(size.x) / 2;
		const auto hy = static_cast<float>(size.y) / 2;

		const auto a_cos = angle.cos();
		const auto a_sin = angle.sin();

		p0 = center + rotate_and_scale(-hx, -hy, a_cos, a_sin, scale.x, scale.y);
		p1 = center + rotate_and_scale(+hx, -hy, a_cos, a_sin, scale.x, scale.y);
		p2 = center + rotate_and_scale(+hx, +hy, a_cos, a_sin, scale.x, scale.y);
		p3 = center + rotate_and_scale(-hx, +hy, a_cos, a_sin, scale.x, scale.y);
	}

	void SpriteBatch::calc_quad_position(
		const Vector2f& center, const Vector2i& size, const Transform2f& tr,
		Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3)
	{
	}

	void SpriteBatch::calc_quad_uv(SpriteBatchEffect effect,
		Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3)
	{
		const auto [x0, x1] =
			effect == SpriteBatchEffect::FlipHorizontal || effect == SpriteBatchEffect::FlipBoth
			? std::make_pair(1.f, 0.f) : std::make_pair(0.f, 1.f);
		const auto [y0, y1] =
			effect == SpriteBatchEffect::FlipVertical || effect == SpriteBatchEffect::FlipBoth
			? std::make_pair(1.f, 0.f) : std::make_pair(0.f, 1.f);

		p0 = Vector2f(x0, y0);
		p1 = Vector2f(x1, y0);
		p2 = Vector2f(x1, y1);
		p3 = Vector2f(x0, y1);
	}

	void SpriteBatch::calc_quad_uv(
		const Vector2i& size, const Recti& rect, SpriteBatchEffect effect,
		Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3)
	{
		const auto w = static_cast<float>(size.x);
		const auto h = static_cast<float>(size.y);

		const float tex_x0 = static_cast<float>(rect.pos.x) / w;
		const float tex_y0 = static_cast<float>(rect.pos.y) / h;
		const float tex_x1 = tex_x0 + static_cast<float>(rect.size.x) / w;
		const float tex_y1 = tex_y0 + static_cast<float>(rect.size.y) / h;

		const auto [x0, x1] =
			effect == SpriteBatchEffect::FlipHorizontal || effect == SpriteBatchEffect::FlipBoth
			? std::make_pair(tex_x1, tex_x0) : std::make_pair(tex_x0, tex_x1);
		const auto [y0, y1] =
			effect == SpriteBatchEffect::FlipVertical || effect == SpriteBatchEffect::FlipBoth
			? std::make_pair(tex_y1, tex_y0) : std::make_pair(tex_y0, tex_y1);

		p0 = Vector2f(x0, y0);
		p1 = Vector2f(x1, y0);
		p2 = Vector2f(x1, y1);
		p3 = Vector2f(x0, y1);
	}
}