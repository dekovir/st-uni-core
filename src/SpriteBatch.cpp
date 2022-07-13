#include "unicore/SpriteBatch.hpp"
#include "unicore/Texture.hpp"
#include "unicore/Font.hpp"
#include "unicore/Sprite.hpp"
#include "unicore/Renderer2D.hpp"

namespace unicore
{
	void SpriteBatch::begin()
	{
		clear();
	}

	void SpriteBatch::end()
	{
		flush();
	}

	void SpriteBatch::clear()
	{
		_batches.clear();
		_current = {};
		_vertices.clear();
	}

	void SpriteBatch::render(Renderer2D& renderer) const
	{
		for (const auto& batch : _batches)
		{
			renderer.draw_triangles(&_vertices[batch.vstart],
				batch.vcount, batch.texture.get());
		}
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const VertexTexColor2& v0, const VertexTexColor2& v1, const VertexTexColor2& v2)
	{
		if (set_texture(texture))
		{
			_vertices.push_back(v0);
			_vertices.push_back(v1);
			_vertices.push_back(v2);
			_current.vcount += 3;
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::draw(const Shared<Texture>& texture,
		const VertexTexColor2& v0, const VertexTexColor2& v1, const VertexTexColor2& v2, const VertexTexColor2& v3)
	{
		if (set_texture(texture))
		{
			_vertices.push_back(v0);
			_vertices.push_back(v1);
			_vertices.push_back(v3);

			_vertices.push_back(v3);
			_vertices.push_back(v1);
			_vertices.push_back(v2);
			_current.vcount += 6;
		}

		return *this;
	}

	static VertexTexColor2 s_quad[4];

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

			return draw(texture, s_quad[0], s_quad[1], s_quad[2], s_quad[3]);
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

			return draw(texture, s_quad[0], s_quad[1], s_quad[2], s_quad[3]);
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

			return draw(texture, s_quad[0], s_quad[1], s_quad[2], s_quad[3]);
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

			return draw(texture, s_quad[0], s_quad[1], s_quad[2], s_quad[3]);
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

			return draw(texture, s_quad[0], s_quad[1], s_quad[2], s_quad[3]);
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

			return draw(texture, s_quad[0], s_quad[1], s_quad[2], s_quad[3]);
		}

		return *this;
	}

	SpriteBatch& SpriteBatch::print(const Shared<Font>& font,
		const Vector2f& pos, StringView text, const Color4b& color)
	{
		if (font)
		{
			Vector2f cur(pos);

			for (size_t i = 0; i < text.size(); i++)
			{
				const auto c = text[i];
				if (auto tex = font->print_char(c, cur, s_quad[0], s_quad[1], s_quad[2], s_quad[3], color))
				{
					draw(tex, s_quad[0], s_quad[1], s_quad[2], s_quad[3]);

					if (i + 1 < text.size())
						cur.x += static_cast<float>(font->find_kerning(c, text[i + 1]));
				}
			}
		}

		return *this;
	}

	void SpriteBatch::flush()
	{
		if (_current.vcount > 0)
		{
			_batches.push_back(_current);

			_current = {};
			_current.vstart = _vertices.size();
		}
	}

	bool SpriteBatch::set_texture(const Shared<Texture>& texture)
	{
		if (texture != nullptr)
		{
			if (_current.texture != texture)
			{
				flush();
				_current.texture = texture;
			}
			return true;
		}

		return false;
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