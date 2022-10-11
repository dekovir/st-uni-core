#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Vector3.hpp"
#include "unicore/Color3.hpp"
#include "unicore/Color4.hpp"
#include "unicore/Vertex.hpp"
#include "unicore/Matrix4.hpp"

namespace unicore
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
		Quads,
		QuadStrip,
		Polygon,
	};

	enum class OGLStateName
	{
	};

	enum class OGLCap
	{
	};

	class IOGLState
	{
	public:
		virtual ~IOGLState() = default;

		virtual void get_boolv(OGLStateName name, Bool* params) = 0;
		virtual void get_intv(OGLStateName name, Int* params) = 0;
		virtual void get_floatv(OGLStateName name, Float* params) = 0;

		virtual void enable(OGLCap cap) = 0;
		virtual void disable(OGLCap cap) = 0;
		virtual Bool is_enabled(OGLCap cap) = 0;
	};

	enum class OGLMatrixMode
	{
		ModelView,
		Projection,
		Texture,
		Color,
	};

	class IOGLMatrixStack
	{
	public:
		virtual ~IOGLMatrixStack() = default;

		virtual void load_identity() = 0;
		virtual void load_matrix(const Float* m) = 0;

		virtual void mult_matrix(const Matrix4f& mat) = 0;
		virtual void mult_matrix(const Float* m) = 0;

		virtual void push_matrix() = 0;
		virtual void pop_matrix() = 0;

		virtual void translate(Float x, Float y, Float z) = 0;
		virtual void translate(const Vector3f& vec) = 0;

		virtual void rotate_x(Radians value) = 0;
		virtual void rotate_y(Radians value) = 0;
		virtual void rotate_z(Radians value) = 0;
		virtual void rotate(Radians x, Radians y, Radians z) = 0;

		virtual void scale(Float x, Float y, Float z) = 0;
		virtual void scale(const Vector3f& vec) = 0;
	};

	class IOGLPipeline : public IOGLMatrixStack
	{
	public:
		virtual ~IOGLPipeline() = default;

		// VERTEX ////////////////////////////////////////////////////////////////////
		virtual void vertex2i(int x, int y) = 0;
		virtual void vertex2i(const Vector2i& v) { vertex3i(v.x, v.y, 0); }
		virtual void vertex2iv(int* v) { vertex3i(v[0], v[1], 0); }

		virtual void vertex2f(Float x, Float y) { vertex3f(x, y, 0); }
		virtual void vertex2f(const Vector2f& v) { vertex3f(v.x, v.y, 0); }
		virtual void vertex2fv(Float* v) { vertex3f(v[0], v[1], 0); }

		virtual void vertex3i(int x, int y, int z);
		virtual void vertex3i(const Vector3i& v) { vertex3i(v.x, v.y, v.z); }
		virtual void vertex3iv(int* v) { vertex3i(v[0], v[1], v[2]); }

		virtual void vertex3f(float x, float y, float z);
		virtual void vertex3f(const Vector3f& v) { vertex3f(v.x, v.y, v.z); }
		virtual void vertex3fv(float* v) { vertex3f(v[0], v[1], v[2]); }

		// TEX_COORD /////////////////////////////////////////////////////////////////
		virtual void tex_coord2i(int u, int v);
		virtual void tex_coord2i(const Vector2i& v) { tex_coord2i(v.x, v.y); }
		virtual void tex_coord2iv(int* v) { tex_coord2i(v[0], v[1]); }

		virtual void tex_coord2f(Float u, Float v);
		virtual void tex_coord2f(const Vector2f& v) { tex_coord2f(v.x, v.y); }
		virtual void tex_coord2fv(Float* v) { tex_coord2f(v[0], v[1]); }

		// COLOR /////////////////////////////////////////////////////////////////////
		virtual void color4b(Byte r, Byte g, Byte b, Byte a);
		virtual void color4b(const Color4b& value) { color4b(value.r, value.g, value.b, value.a); }
		virtual void color4bv(Byte* v) { color4b(v[0], v[1], v[2], v[3]); }

		virtual void color4f(Float r, Float g, Float b, Float a);
		virtual void color4f(const Color4f& value) { color4f(value.r, value.g, value.b, value.a); }
		virtual void color4fv(Float* v) { color4f(v[0], v[1], v[2], v[3]); }

		virtual void color3b(Byte r, Byte g, Byte b) { color4b(r, g, b, 0xFF); }
		virtual void color3b(const Color3b& value) { color4b(value.r, value.g, value.b, 1); }
		virtual void color3bv(Byte* v) { color4b(v[0], v[1], v[2], 1); }

		virtual void color3f(Float r, Float g, Float b) { color4f(r, g, b, 1); }
		virtual void color3f(const Color3f& value) { color4f(value.r, value.g, value.b, 1); }
		virtual void color3fv(Float* v) { color4f(v[0], v[1], v[2], 1); }
	};

	enum class OGLClearBit : uint8_t
	{
		Color = 1 << 0,
		Depth = 1 << 1,
		Stencil = 1 << 2,
	};
	UNICORE_ENUM_FLAGS(OGLClearBit, OGLClearMask);

	class IRendererOGL : public IOGLState, public IOGLPipeline
	{
	public:
		virtual ~IRendererOGL() = default;

		virtual void clear_color(Float r, Float g, Float b, Float a) = 0;
		virtual void clear_color(const Color4f& color) { clear_color(color.r, color.g, color.b, color.a); }

		virtual void clear_depth(Float depth) = 0;
		virtual void clear_stencil(Int s) = 0;

		virtual void clear(OGLClearMask mask) = 0;

		virtual void scissor(Int x, Int y, Int width, Int height) = 0;
		virtual void scissor(const Recti& rect) { scissor(rect.x, rect.y, rect.w, rect.h); }

		virtual void viewport(Int x, Int y, Int width, Int height) = 0;
		virtual void viewport(const Recti& rect) { viewport(rect.x, rect.y, rect.w, rect.h); }
	};

	class RendererOGL
	{
	public:
		void begin(RenderMode mode);
		void end();
		void clear();

		// MATRIX ////////////////////////////////////////////////////////////////////
		void load_identity();
		// TODO: Replace with span
		void load_matrix(const Float* m);

		void mult_matrix(const Matrix4f& mat);
		// TODO: Replace with span
		void mult_matrix(const Float* m);

		void push_matrix();
		void pop_matrix();

		void translate(Float x, Float y, Float z);
		void translate(const Vector3f& vec) { translate(vec.x, vec.y, vec.z); }

		void rotate_x(Radians value);
		void rotate_y(Radians value);
		void rotate_z(Radians value);

		void scale(Float x, Float y, Float z);
		void scale(const Vector3f& vec) { scale(vec.x, vec.y, vec.z); }

		// VERTEX ////////////////////////////////////////////////////////////////////
		void vertex3i(int x, int y, int z);
		void vertex3i(const Vector3i& v) { vertex3i(v.x, v.y, v.z); }
		// TODO: Replace with span
		void vertex3iv(int* v) { vertex3i(v[0], v[1], v[2]); }

		void vertex3f(float x, float y, float z);
		void vertex3f(const Vector3f& v) { vertex3f(v.x, v.y, v.z); }
		// TODO: Replace with span
		void vertex3fv(float* v) { vertex3f(v[0], v[1], v[2]); }

		void vertex2i(int x, int y) { vertex3i(x, y, 0); }
		void vertex2i(const Vector2i& v) { vertex3i(v.x, v.y, 0); }
		// TODO: Replace with span
		void vertex2iv(int* v) { vertex3i(v[0], v[1], 0); }

		void vertex2f(Float x, Float y) { vertex3f(x, y, 0); }
		void vertex2f(const Vector2f& v) { vertex3f(v.x, v.y, 0); }
		// TODO: Replace with span
		void vertex2fv(Float* v) { vertex3f(v[0], v[1], 0); }

		// TEX_COORD /////////////////////////////////////////////////////////////////
		void tex_coord2i(int u, int v);
		void tex_coord2i(const Vector2i& v) { tex_coord2i(v.x, v.y); }
		// TODO: Replace with span
		void tex_coord2iv(int* v) { tex_coord2i(v[0], v[1]); }

		void tex_coord2f(Float u, Float v);
		void tex_coord2f(const Vector2f& v) { tex_coord2f(v.x, v.y); }
		// TODO: Replace with span
		void tex_coord2fv(Float* v) { tex_coord2f(v[0], v[1]); }

		// COLOR /////////////////////////////////////////////////////////////////////
		void color4b(Byte r, Byte g, Byte b, Byte a);
		void color4b(const Color4b& value) { color4b(value.r, value.g, value.b, value.a); }
		// TODO: Replace with span
		void color4bv(Byte* v) { color4b(v[0], v[1], v[2], v[3]); }

		void color4f(Float r, Float g, Float b, Float a);
		void color4f(const Color4f& value) { color4f(value.r, value.g, value.b, value.a); }
		// TODO: Replace with span
		void color4fv(Float* v) { color4f(v[0], v[1], v[2], v[3]); }

		void color3b(Byte r, Byte g, Byte b) { color4b(r, g, b, 0xFF); }
		void color3b(const Color3b& value) { color4b(value.r, value.g, value.b, 1); }
		// TODO: Replace with span
		void color3bv(Byte* v) { color4b(v[0], v[1], v[2], 1); }

		void color3f(Float r, Float g, Float b) { color4f(r, g, b, 1); }
		void color3f(const Color3f& value) { color4f(value.r, value.g, value.b, 1); }
		// TODO: Replace with span
		void color3fv(Float* v) { color4f(v[0], v[1], v[2], 1); }

	protected:
		struct BatchInfo
		{
			RenderMode mode = RenderMode::Points;
			size_t start = 0;
			size_t count = 0;
		};

		List<BatchInfo> _batches;
		BatchInfo _batch;

		List<VertexTexColor3> _vertices;
		VertexTexColor3 _current;

		static constexpr size_t MAX_STACK_DEPTH = 32;

		Matrix4f _matrix_stack[MAX_STACK_DEPTH]{};
		UInt8 _stack_index = 0;

		Matrix4f& current_matrix() { return _matrix_stack[_stack_index]; }

		void flush_batch();
	};
}