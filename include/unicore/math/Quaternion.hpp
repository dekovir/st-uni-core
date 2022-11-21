#pragma once
#include "unicore/math/Matrix3.hpp"

namespace unicore
{
	class Quaternion
	{
	public:
		using T = Float;

		T x, y, z, w;

		constexpr Quaternion() noexcept
			: x(0), y(0), z(0), w(0)
		{}

		constexpr Quaternion(T x_, T y_, T z_, T w_) noexcept
			: x(x_), y(y_), z(z_), w(w_)
		{}

		// TODO: Replace with span
		constexpr Quaternion(const T* v) noexcept
			: x(v[0]), y(v[1]), z(v[2]), w(v[3])
		{}

		constexpr Quaternion(const Quaternion& other) noexcept = default;
		constexpr Quaternion(Quaternion&& other) noexcept = default;

		~Quaternion() = default;

		UC_NODISCARD constexpr size_t size() const { return 4; }

		T& operator[](int index) { return (&x)[index]; }
		T operator[](int index) const { return (&x)[index]; }

		Quaternion& operator=(const Quaternion& other) noexcept = default;
		Quaternion& operator=(Quaternion&& other) noexcept = default;

		Quaternion& operator+=(const Quaternion& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		Quaternion& operator-=(const Quaternion& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		Quaternion& operator*=(Float value)
		{
			x *= value;
			y *= value;
			z *= value;
			w *= value;
			return *this;
		}

		Quaternion& operator/=(Float value)
		{
			x /= value;
			y /= value;
			z /= value;
			w /= value;
			return *this;
		}

		UC_NODISCARD Angles3f euler_angles() const
		{
			const float check = 2.0f * (-y * z + w * x);

			if (check < -0.995f)
			{
				return {
					-90.0_deg,
					0.0_rad,
					Radians(-atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)))
				};
			}
			if (check > 0.995f)
			{
				return {
					90.0_deg,
					0.0_rad,
					Radians(atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)))
				};
			}

			return {
				Radians(asinf(check)),
				Radians(atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y))),
				Radians(atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z)))
			};
		}

		UC_NODISCARD Vector3<T> axis() const
		{
			return Vector3(x, y, z) / Math::sqrt(1.f - w * w);
		}

		UC_NODISCARD Radians angle() const
		{
			return Radians(2.f * Math::acos(w));
		}

		UC_NODISCARD constexpr Matrix3<T> rotation_matrix() const
		{
			return
			{
				1.0f - 2.0f * y * y - 2.0f * z * z,
				2.0f * x * y - 2.0f * w * z,
				2.0f * x * z + 2.0f * w * y,
				2.0f * x * y + 2.0f * w * z,
				1.0f - 2.0f * x * x - 2.0f * z * z,
				2.0f * y * z - 2.0f * w * x,
				2.0f * x * z - 2.0f * w * y,
				2.0f * y * z + 2.0f * w * x,
				1.0f - 2.0f * x * x - 2.0f * y * y
			};
		}

		static Quaternion lerp(const Quaternion& a, const Quaternion& b, Float t)
		{
		}

		static Quaternion from_axis(const Vector3<T>& axis, Radians angle)
		{
			const auto [sin_val, cos_val] = angle.sin_cos();
			return {
				axis.x * sin_val,
				axis.y * sin_val,
				axis.z * sin_val,
				cos_val
			};
		}

		static Quaternion from_euler(Radians x, Radians y, Radians z)
		{
			const auto [sin_x, cos_x] = x.sin_cos();
			const auto [sin_y, cos_y] = y.sin_cos();
			const auto [sin_z, cos_z] = z.sin_cos();

			return {
				cos_y * sin_x * cos_z + sin_y * cos_x * sin_z,
				sin_y * cos_x * cos_z - cos_y * sin_x * sin_z,
				cos_y * cos_x * sin_z - sin_y * sin_x * cos_z,
				cos_y * cos_x * cos_z + sin_y * sin_x * sin_z
			};
		}

		static Quaternion from_euler(const Angles3f& angles)
		{
			return from_euler(angles.x, angles.y, angles.z);
		}
	};

	static_assert(sizeof(Quaternion) == sizeof(float) * 4);
	static_assert(std::is_polymorphic_v<Quaternion> == false);

	// OPERATORS ///////////////////////////////////////////////////////////////
	static constexpr bool operator==(const Quaternion& a, const Quaternion& b)
	{
		return
			Math::equals(a.x, b.x) &&
			Math::equals(a.y, b.y) &&
			Math::equals(a.z, b.z) &&
			Math::equals(a.w, b.w);
	}

	static constexpr bool operator!=(const Quaternion& a, const Quaternion& b)
	{
		return !(a == b);
	}

	static constexpr Quaternion operator+(const Quaternion& a, const Quaternion& b)
	{
		return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
	}

	static constexpr Quaternion operator-(const Quaternion& a, const Quaternion& b)
	{
		return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
	}

	static constexpr Quaternion operator*(const Quaternion& q, Float k)
	{
		return { q.x * k, q.y * k, q.z * k, q.w * k };
	}

	static constexpr Quaternion operator/(const Quaternion& q, Float k)
	{
		return { q.x / k, q.y / k, q.z / k, q.w / k };
	}

	static constexpr Quaternion operator*(Float k, const Quaternion& q)
	{
		return { q.x * k, q.y * k, q.z * k, q.w * k };
	}

	static constexpr Vector3<Float> operator*(const Quaternion& q, const Vector3<Float>& vector)
	{
	}

	// CONST /////////////////////////////////////////////////////////////////////
	struct QuaternionConst
	{
		static constexpr Quaternion Zero = Quaternion();
	};
}