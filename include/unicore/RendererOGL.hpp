#pragma once
#include "unicore/renderer/ogl1/Pipeline.hpp"

namespace unicore::ogl1
{
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