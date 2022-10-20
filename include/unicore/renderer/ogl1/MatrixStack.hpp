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

		virtual void translate3f(Float x, Float y, Float z) { mult_matrix(Matrix4f::translate(x, y, z)); }
		virtual void translate3f(const Vector3f& vec) { translate3f(vec.x, vec.y, vec.z); }
		// TODO: Replace with span
		virtual void translate3fv(const Float* v) { translate3f(v[0], v[1], v[2]); }

		virtual void rotate_xf(Radians angle) { mult_matrix(Matrix4f::rotate_x(angle)); }
		virtual void rotate_yf(Radians angle) { mult_matrix(Matrix4f::rotate_y(angle)); }
		virtual void rotate_zf(Radians angle) { mult_matrix(Matrix4f::rotate_z(angle)); }

		virtual void rotate3f(Radians x, Radians y, Radians z) = 0;
		virtual void rotate3f(const Angles3f& angles) { rotate3f(angles.x, angles.y, angles.z); }
		// TODO: Replace with span
		virtual void rotate3fv(const Radians* v) { rotate3f(v[0], v[1], v[2]); }

		virtual void scale3f(Float x, Float y, Float z) { mult_matrix(Matrix4f::scale(x, y, z)); }
		virtual void scale3f(const Vector3f& vec) { scale3f(vec.x, vec.y, vec.z); }
		// TODO: Replace with span
		virtual void scale3fv(const Float* v) { scale3f(v[0], v[1], v[2]); }
	};
}