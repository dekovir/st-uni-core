#pragma once
#include "unicore/math/Vector2.hpp"
#include "unicore/math/Vector3.hpp"
#include "unicore/renderer/Color4.hpp"

namespace unicore
{
	// VertexBase /////////////////////////////////////////////////////////////////
	template<typename TPos>
	struct VertexBase
	{
		TPos pos;

		constexpr VertexBase() = default;
		constexpr VertexBase(const TPos& pos_) : pos(pos_) {}

		UC_TYPE_DEFAULT_DTOR_MOVE_COPY(VertexBase);
	};

	using VertexBase2f = VertexBase<Vector2f>;
	using VertexBase3f = VertexBase<Vector3f>;

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