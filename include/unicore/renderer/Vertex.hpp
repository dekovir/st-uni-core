#pragma once
#include "unicore/math/Vector2.hpp"
#include "unicore/math/Vector3.hpp"
#include "unicore/renderer/Color4.hpp"

namespace unicore
{
	// VertexPosition /////////////////////////////////////////////////////////////
	template<typename TPos>
	struct VertexPosition
	{
		TPos pos;

		constexpr VertexPosition() = default;
		constexpr VertexPosition(const TPos& pos_) : pos(pos_) {}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexPosition);
	};

	using VertexBase2f = VertexPosition<Vector2f>;
	using VertexBase3f = VertexPosition<Vector3f>;

	// VertexColor ////////////////////////////////////////////////////////////////
	template<typename TPos, typename TColor>
	struct VertexColor
	{
		TPos pos;
		TColor col;

		constexpr VertexColor() = default;

		explicit constexpr VertexColor(const TPos& pos_)
			: pos(pos_), col({})
		{}

		constexpr VertexColor(const TPos& pos_, const TColor& color_)
			: pos(pos_), col(color_)
		{}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexColor);
	};

	using VertexColor2f = VertexColor<Vector2f, Color4b>;
	using VertexColor3f = VertexColor<Vector3f, Color4b>;

	// VertexColor ////////////////////////////////////////////////////////////////
	template<typename TPos, typename TTex>
	struct VertexTexture
	{
		TPos pos;
		TTex tex;

		constexpr VertexTexture() = default;

		explicit constexpr VertexTexture(const TPos& pos_)
			: pos(pos_), tex({})
		{}

		constexpr VertexTexture(const TPos& pos_, const TTex& tex_)
			: pos(pos_), tex(tex_)
		{}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexTexture);
	};

	using VertexTexture2f = VertexTexture<Vector2f, Vector2f>;
	using VertexTexture3f = VertexTexture<Vector3f, Vector2f>;

	// VertexNormal ///////////////////////////////////////////////////////////////
	template<typename TPos, typename TNormal>
	struct VertexNormal
	{
		TPos pos;
		TNormal normal;

		constexpr VertexNormal() = default;

		explicit constexpr VertexNormal(const TPos& pos_)
			: pos(pos_), normal({})
		{}

		constexpr VertexNormal(const TPos& pos_, const TNormal& normal_)
			: pos(pos_), normal(normal_)
		{}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexNormal);
	};

	using VertexNormal2f = VertexNormal<Vector2f, Vector2f>;
	using VertexNormal3f = VertexNormal<Vector3f, Vector3f>;

	// VertexColorTexture /////////////////////////////////////////////////////////
	template<typename TPos, typename TTexCoords, typename TColor>
	struct VertexColorTexture
	{
		TPos pos;
		TColor col;
		TTexCoords tex;

		constexpr VertexColorTexture() = default;

		explicit constexpr VertexColorTexture(const TPos& pos_)
			: pos(pos_), col({}), tex({})
		{}

		constexpr VertexColorTexture(const TPos& pos_, const TColor& color_)
			: pos(pos_), col(color_), tex({})
		{}

		constexpr VertexColorTexture(const TPos& pos_, const TColor& color_, const TTexCoords& uv_)
			: pos(pos_), col(color_), tex(uv_)
		{}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexColorTexture);
	};

	using VertexColorTexture2f = VertexColorTexture<Vector2f, Vector2f, Color4b>;
	using VertexColorTexture3f = VertexColorTexture<Vector3f, Vector2f, Color4b>;

	// VertexNormalColor //////////////////////////////////////////////////////////
	template<typename TPos, typename TNormal, typename TColor>
	struct VertexNormalColor
	{
		TPos pos;
		TNormal normal;
		TColor color;

		constexpr VertexNormalColor() = default;

		explicit constexpr VertexNormalColor(const TPos& pos_)
			: pos(pos_), normal({}), color({})
		{}

		constexpr VertexNormalColor(const TPos& pos_, const TNormal& normal_)
			: pos(pos_), normal(normal_), color({})
		{}

		constexpr VertexNormalColor(const TPos& pos_, const TNormal& normal_, const TColor& color_)
			: pos(pos_), normal(normal_), color(color_)
		{}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexNormalColor);
	};

	using VertexNormalColor2f = VertexNormalColor<Vector2f, Vector2f, Color4b>;
	using VertexNormalColor3f = VertexNormalColor<Vector3f, Vector3f, Color4b>;

	// VertexNormalTexture ///////////////////////////////////////////////////////
	template<typename TPos, typename TNormal, typename TTexture>
	struct VertexNormalTexture
	{
		TPos pos;
		TNormal normal;
		TTexture uv;

		constexpr VertexNormalTexture() = default;

		explicit constexpr VertexNormalTexture(const TPos& pos_)
			: pos(pos_), normal({}), uv({})
		{}

		constexpr VertexNormalTexture(const TPos& pos_, const TNormal& normal_)
			: pos(pos_), normal(normal_), uv({})
		{}

		constexpr VertexNormalTexture(const TPos& pos_, const TNormal& normal_, const TTexture& uv_)
			: pos(pos_), normal(normal_), uv(uv_)
		{}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexNormalTexture);
	};

	using VertexNormalTexture2f = VertexNormalTexture<Vector2f, Vector2f, Vector2f>;
	using VertexNormalTexture3f = VertexNormalTexture<Vector3f, Vector3f, Vector2f>;

	// VertexNormalColorTexture //////////////////////////////////////////////////////////
	template<typename TPos, typename TNormal, typename TColor, typename TTexture>
	struct VertexNormalColorTexture
	{
		TPos pos;
		TNormal normal;
		TColor color;
		TTexture uv;

		constexpr VertexNormalColorTexture() = default;

		explicit constexpr VertexNormalColorTexture(const TPos& pos_)
			: pos(pos_), normal({}), color({}), uv({})
		{}

		constexpr VertexNormalColorTexture(const TPos& pos_, const TNormal& normal_)
			: pos(pos_), normal(normal_), color({}), uv({})
		{}

		constexpr VertexNormalColorTexture(const TPos& pos_, const TNormal& normal_, const TColor& color_)
			: pos(pos_), normal(normal_), color(color_), uv({})
		{}

		constexpr VertexNormalColorTexture(const TPos& pos_, const TNormal& normal_, const TColor& color_, const TTexture& uv_)
			: pos(pos_), normal(normal_), color(color_), uv(uv_)
		{}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexNormalColorTexture);
	};

	using VertexNormalColorTexture2f = VertexNormalColorTexture<Vector2f, Vector2f, Color4b, Vector2f>;
	using VertexNormalColorTexture3f = VertexNormalColorTexture<Vector3f, Vector3f, Color4b, Vector2f>;

	// QUAD ///////////////////////////////////////////////////////////////////////
	template<typename TVertex>
	struct Quad
	{
		TVertex v[4];
	};

	using QuadColor2f = Quad<VertexColor2f>;
	using QuadColor3f = Quad<VertexColor3f>;

	using QuadColorTexture2f = Quad<VertexColorTexture2f>;
	using QuadColorTexture3f = Quad<VertexColorTexture3f>;
}