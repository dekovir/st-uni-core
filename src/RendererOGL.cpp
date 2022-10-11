#include "unicore/RendererOGL.hpp"

namespace unicore
{
	void RendererOGL::begin(RenderMode mode)
	{
		if (_batch.mode != mode)
		{
			flush_batch();
			_batch.mode = mode;
		}
	}

	void RendererOGL::end()
	{
		flush_batch();
	}

	void RendererOGL::clear()
	{
		_batches.clear();
		_batch = {};

		_vertices.clear();
		_stack_index = 0;

		load_identity();
	}

	// MATRIX ////////////////////////////////////////////////////////////////////
	void RendererOGL::load_identity()
	{
		current_matrix() = MatrixConst4f::Identity;
	}

	void RendererOGL::load_matrix(const Float* m)
	{
		current_matrix() = Matrix4f(m);
	}

	void RendererOGL::mult_matrix(const Matrix4f& mat)
	{
		current_matrix() *= mat;
	}

	void RendererOGL::mult_matrix(const Float* m)
	{
		current_matrix() *= Matrix4f(m);
	}

	void RendererOGL::push_matrix()
	{
		_matrix_stack[_stack_index + 1] = _matrix_stack[_stack_index];
		_stack_index++;
	}

	void RendererOGL::pop_matrix()
	{
		_stack_index--;
	}

	void RendererOGL::translate(Float x, Float y, Float z)
	{
		mult_matrix(Matrix4f::translate(x, y, z));
	}

	void RendererOGL::rotate_x(Radians value)
	{
		mult_matrix(Matrix4f::rotate_x(value));
	}

	void RendererOGL::rotate_y(Radians value)
	{
		mult_matrix(Matrix4f::rotate_y(value));
	}

	void RendererOGL::rotate_z(Radians value)
	{
		mult_matrix(Matrix4f::rotate_z(value));
	}

	void RendererOGL::scale(Float x, Float y, Float z)
	{
		mult_matrix(Matrix4f::scale(x, y, z));
	}

	// VERTEX ////////////////////////////////////////////////////////////////////
	void RendererOGL::vertex3i(int x, int y, int z)
	{
		_current.pos.set(x, y, z);
		_current.pos = current_matrix() * _current.pos;
		_vertices.push_back(_current);
	}

	void RendererOGL::vertex3f(float x, float y, float z)
	{
		_current.pos.set(x, y, z);
		_vertices.push_back(_current);
	}

	void RendererOGL::tex_coord2i(int u, int v)
	{
		_current.uv.set(u, v);
	}

	void RendererOGL::tex_coord2f(Float u, Float v)
	{
		_current.uv.set(u, v);
	}

	void RendererOGL::color4b(Byte r, Byte g, Byte b, Byte a)
	{
		_current.col = Color4b(r, g, b, a);
	}

	void RendererOGL::color4f(Float r, Float g, Float b, Float a)
	{
		//_current.col = Color4f(r, g, b, a).ca;
	}

	void RendererOGL::flush_batch()
	{
		if (_batch.count > 0)
		{
			_batches.push_back(_batch);

			_batch.mode = RenderMode::Points;
			_batch.start = _vertices.size();
			_batch.count = 0;
		}
	}
}