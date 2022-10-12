#pragma once
#include "unicore/math/Vector3.hpp"
#include "unicore/math/Matrix4.hpp"

namespace unicore::ogl1
{
	enum class MatrixMode
	{
		ModelView,
		Projection,
		Texture,
		Color,
	};

	class MatrixStack
	{
	public:
		virtual ~MatrixStack() = default;

		virtual void matrix_mode(MatrixMode mode) = 0;
		UC_NODISCARD virtual MatrixMode matrix_mode() const = 0;

		virtual void load_identity() = 0;

		virtual void load_matrix(const Matrix4f& mat) = 0;
		// TODO: Replace with span
		virtual void load_matrix(const Float* m) = 0;

		virtual void mult_matrix(const Matrix4f& mat) = 0;
		// TODO: Replace with span
		virtual void mult_matrix(const Float* m) = 0;

		virtual void push_matrix() = 0;
		virtual void pop_matrix() = 0;

		virtual void translate(Float x, Float y, Float z) { mult_matrix(Matrix4f::translate(x, y, z)); }
		virtual void translate(const Vector3f& vec) { translate(vec.x, vec.y, vec.z); }

		virtual void rotate_x(Radians angle) { mult_matrix(Matrix4f::rotate_x(angle)); }
		virtual void rotate_y(Radians angle) { mult_matrix(Matrix4f::rotate_y(angle)); }
		virtual void rotate_z(Radians angle) { mult_matrix(Matrix4f::rotate_z(angle)); }
		//virtual void rotate(Radians x, Radians y, Radians z) = 0;

		virtual void scale(Float x, Float y, Float z) { mult_matrix(Matrix4f::scale(x, y, z)); }
		virtual void scale(const Vector3f& vec) { scale(vec.x, vec.y, vec.z); }
	};
}