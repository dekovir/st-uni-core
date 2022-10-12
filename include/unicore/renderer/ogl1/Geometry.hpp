#pragma once
#include "unicore/Color3.hpp"
#include "unicore/Color4.hpp"
#include "unicore/Vertex.hpp"
#include "unicore/renderer/ogl1/MatrixStack.hpp"

namespace unicore::ogl1
{
	enum class RenderMode
	{
		Points,
		Lines,
		LineStrip,
		LineLoop,
		Triangles,
		TriangleStrip,
		TriangleFan,
	};

	class Geometry : public MatrixStack
	{
	public:
		virtual void begin(RenderMode mode) = 0;
		virtual void end() = 0;

		// VERTEX ////////////////////////////////////////////////////////////////////
		virtual void vertex2i(Int x, Int y) = 0;
		virtual void vertex2i(const Vector2i& v) { vertex3i(v.x, v.y, 0); }
		// TODO: Replace with span
		virtual void vertex2iv(const Int* v) { vertex3i(v[0], v[1], 0); }

		virtual void vertex2f(Float x, Float y) = 0;
		virtual void vertex2f(const Vector2f& v) { vertex2f(v.x, v.y); }
		// TODO: Replace with span
		virtual void vertex2fv(const Float* v) { vertex2f(v[0], v[1]); }

		virtual void vertex3i(Int x, Int y, Int z) = 0;
		virtual void vertex3i(const Vector3i& v) { vertex3i(v.x, v.y, v.z); }
		// TODO: Replace with span
		virtual void vertex3iv(const Int* v) { vertex3i(v[0], v[1], v[2]); }

		virtual void vertex3f(Float x, Float y, Float z) = 0;
		virtual void vertex3f(const Vector3f& v) { vertex3f(v.x, v.y, v.z); }
		// TODO: Replace with span
		virtual void vertex3fv(const Float* v) { vertex3f(v[0], v[1], v[2]); }

		// TEX_COORD /////////////////////////////////////////////////////////////////
		virtual void tex_coord2i(Int u, Int v) = 0;
		virtual void tex_coord2i(const Vector2i& v) { tex_coord2i(v.x, v.y); }
		// TODO: Replace with span
		virtual void tex_coord2iv(const Int* v) { tex_coord2i(v[0], v[1]); }

		virtual void tex_coord2f(Float u, Float v) = 0;
		virtual void tex_coord2f(const Vector2f& v) { tex_coord2f(v.x, v.y); }
		// TODO: Replace with span
		virtual void tex_coord2fv(const Float* v) { tex_coord2f(v[0], v[1]); }

		// COLOR /////////////////////////////////////////////////////////////////////
		virtual void color4b(Byte r, Byte g, Byte b, Byte a) = 0;
		virtual void color4b(const Color4b& value) { color4b(value.r, value.g, value.b, value.a); }
		// TODO: Replace with span
		virtual void color4bv(const Byte* v) { color4b(v[0], v[1], v[2], v[3]); }

		virtual void color4f(Float r, Float g, Float b, Float a) = 0;
		virtual void color4f(const Color4f& value) { color4f(value.r, value.g, value.b, value.a); }
		// TODO: Replace with span
		virtual void color4fv(const Float* v) { color4f(v[0], v[1], v[2], v[3]); }

		virtual void color3b(Byte r, Byte g, Byte b) { color4b(r, g, b, 0xFF); }
		virtual void color3b(const Color3b& value) { color4b(value.r, value.g, value.b, 1); }
		// TODO: Replace with span
		virtual void color3bv(const Byte* v) { color4b(v[0], v[1], v[2], 1); }

		virtual void color3f(Float r, Float g, Float b) { color4f(r, g, b, 1); }
		virtual void color3f(const Color3f& value) { color4f(value.r, value.g, value.b, 1); }
		// TODO: Replace with span
		virtual void color3fv(const Float* v) { color4f(v[0], v[1], v[2], 1); }
	};
}