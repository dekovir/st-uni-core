#include "unicore/SpriteBatch.hpp"
#include "unicore/math/Transform2.hpp"
#include "unicore/Texture.hpp"
#include "unicore/Font.hpp"
#include "unicore/Sprite.hpp"

namespace unicore
{
	static VertexTexColor2 s_quad[4];
	static List<VertexColorQuad2> s_quad_list;
	static Dictionary<Shared<Texture>, List<VertexTexColorQuad2>> s_quad_dict;

	static void convert(const VertexColor2& from, VertexTexColor2& to)
	{
		to.pos = from.pos;
		to.col = from.col;
	}

	void SpriteBatch::render(sdl2::PipelineRender& renderer) const
	{
		for (const auto& batch : _batches)
		{
			renderer.draw_triangles(&_vertices[batch.start],
				batch.count, batch.texture.get());
		}
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
		const VertexTexColor2& v0, const VertexTexColor2& v1, const VertexTexColor2& v2,
		const Shared<Texture>& texture)
	{
		set_texture(texture);

		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
		_current.count += 3;

		return *this;
	}

	SpriteBatch& SpriteBatch::draw_tri(const VertexTexColor2* arr,
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
		const VertexTexColor2& v0, const VertexTexColor2& v1,
		const VertexTexColor2& v2, const VertexTexColor2& v3,
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

	SpriteBatch& SpriteBatch::draw_quad(const VertexTexColor2* arr,
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

		s_quad[0].pos = Vector2f(rect.x, rect.y);
		s_quad[1].pos = Vector2f(rect.x + rect.w, rect.y);
		s_quad[2].pos = Vector2f(rect.x + rect.w, rect.y + rect.h);
		s_quad[3].pos = Vector2f(rect.x, rect.y + rect.h);

		if (uv.has_value())
		{
			const auto r = uv.value();
			s_quad[0].uv = Vector2f(r.x, r.y);
			s_quad[1].uv = Vector2f(r.x + r.w, r.y);
			s_quad[2].uv = Vector2f(r.x + r.w, r.y + r.h);
			s_quad[3].uv = Vector2f(r.x, r.y + r.h);
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
		const Vector2f& center, const Color4b& color)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(),
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

			s_quad[0].uv = Vector2f(0, 0);
			s_quad[1].uv = Vector2f(1, 0);
			s_quad[2].uv = Vector2f(1, 1);
			s_quad[3].uv = Vector2f(0, 1);

			s_quad[0].col = color;
			s_quad[1].col = color;
			s_quad[2].col = color;
			s_quad[3].col = color;

			return draw_quad(s_quad, texture);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const Vector2f& center, const Recti& part, const Color4b& color)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(),
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);
			calc_quad_uv(texture->size(), part,
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
		const Vector2f& center, const Radians& angle,
		const Vector2f& scale, const Color4b& color)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(), angle, scale,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

			s_quad[0].uv = Vector2f(0, 0);
			s_quad[1].uv = Vector2f(1, 0);
			s_quad[2].uv = Vector2f(1, 1);
			s_quad[3].uv = Vector2f(0, 1);

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
		const Radians& angle, const Vector2f& scale, const Color4b& color)
	{
		if (texture)
		{
			calc_quad_position(center, texture->size(), angle, scale,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

			calc_quad_uv(texture->size(), part,
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
		const Vector2f& center, const Color4b& color)
	{
		if (sprite)
		{
			auto& rect = sprite->rect();
			auto& texture = sprite->texture();

			calc_quad_position(center, rect.size(),
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);
			calc_quad_uv(texture->size(), rect,
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
		const Vector2f& center, const Radians& angle,
		const Vector2f& scale, const Color4b& color)
	{
		if (sprite)
		{
			auto& rect = sprite->rect();
			auto& texture = sprite->texture();

			calc_quad_position(center, rect.size(), angle, scale,
				s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);
			calc_quad_uv(texture->size(), rect,
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
		const Transform2& tr, StringView32 text, const Color4b& color)
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

	void SpriteBatch::calc_quad_uv(const Vector2i& size, const Recti& rect,
		Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3)
	{
		const auto w = static_cast<float>(size.x);
		const auto h = static_cast<float>(size.y);

		const float tx1 = static_cast<float>(rect.x) / w;
		const float ty1 = static_cast<float>(rect.y) / h;
		const float tx2 = tx1 + static_cast<float>(rect.w) / w;
		const float ty2 = ty1 + static_cast<float>(rect.h) / h;

		p0.set(tx1, ty1);
		p1.set(tx2, ty1);
		p2.set(tx2, ty2);
		p3.set(tx1, ty2);
	}
}