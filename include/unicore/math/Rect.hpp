#pragma once
#include "unicore/math/Hash.hpp"
#include "unicore/math/Range.hpp"
#include "unicore/math/Vector2.hpp"
#include "unicore/system/StringBuilder.hpp"

namespace unicore
{
	template<typename T>
	struct Rect
	{
		Vector2<T> pos;
		Vector2<T> size;

		constexpr Rect() = default;
		constexpr Rect(T x, T y, T w, T h);
		constexpr Rect(T x, T y, const Vector2<T>& size);
		constexpr Rect(const Vector2<T>& position, T w, T h);
		constexpr Rect(const Vector2<T>& position, const Vector2<T>& size);

		// Copy constructor
		constexpr Rect(const Rect& other) = default;

		// Move constructor
		constexpr Rect(Rect&& other) noexcept
			: pos(other.pos), size(other.size) {}

		~Rect() = default;

		// Copy assignment operator
		Rect& operator=(const Rect& other) noexcept = default;

		// Move assignment operator
		Rect& operator=(Rect&& other) noexcept
		{
			pos = other.pos;
			size = other.size;
			return *this;
		}

		void set(T x, T y, T w, T h)
		{
			pos.set(x, y);
			size.set(w, h);
		}

		void set(const Vector2<T>& pos_, const Vector2<T>& size_)
		{
			pos = pos_;
			size = size_;
		}

		UC_NODISCARD constexpr T min_x() const { return pos.x; }
		UC_NODISCARD constexpr T max_x() const { return pos.x + size.x; }

		UC_NODISCARD constexpr T min_y() const { return pos.y; }
		UC_NODISCARD constexpr T max_y() const { return pos.y + size.y; }

		UC_NODISCARD constexpr Range<T> range_x() const { return { min_x(), max_x() }; }
		UC_NODISCARD constexpr Range<T> range_y() const { return { min_y(), max_y() }; }

		UC_NODISCARD constexpr T left() const { return pos.x; }
		UC_NODISCARD constexpr T right() const { return pos.x + size.x; }
		UC_NODISCARD constexpr T bottom() const { return pos.y; }
		UC_NODISCARD constexpr T top() const { return pos.y + size.y; }

		UC_NODISCARD constexpr Vector2<T> center() const { return pos + size / 2; }

		UC_NODISCARD constexpr Vector2<T> top_left() const { return Vector2<T>(pos.x, pos.y + size.y); }
		UC_NODISCARD constexpr Vector2<T> top_right() const { return Vector2<T>(pos.x + size.x, pos.y + size.y); }

		UC_NODISCARD constexpr Vector2<T> bottom_left() const { return Vector2<T>(pos.x, pos.y); }
		UC_NODISCARD constexpr Vector2<T> bottom_right() const { return Vector2<T>(pos.x + size.x, pos.y); }

		void push_points(List<Vector2<T>>& points) const
		{
			points.emplace_back(bottom_left());
			points.emplace_back(bottom_right());
			points.emplace_back(top_right());
			points.emplace_back(top_left());
		}

		UC_NODISCARD constexpr bool contains(T x_, T y_) const
		{
			return
				(x_ >= min_x()) && (x_ < max_x()) &&
				(y_ >= min_y()) && (y_ < max_y());
		}

		UC_NODISCARD constexpr bool contains(const Vector2<T>& point) const
		{
			return
				(point.x >= min_x()) && (point.x < max_x()) &&
				(point.y >= min_y()) && (point.y < max_y());
		}

		UC_NODISCARD constexpr bool overlaps(const Rect& other) const
		{
			return
				other.max_x() > min_x() &&
				other.min_x() < max_x() &&
				other.max_y() > min_y() &&
				other.min_y() < max_y();
		}

		UC_NODISCARD constexpr Optional<Rect> intersection(const Rect& rectangle) const
		{
			// Compute the intersection boundaries
			const T inter_left = Math::max(min_x(), rectangle.min_x());
			const T inter_top = Math::max(min_y(), rectangle.min_y());
			const T inter_right = Math::min(max_x(), rectangle.max_x());
			const T inter_bottom = Math::min(max_y(), rectangle.max_y());

			// If the intersection is valid (positive non zero area), then there is an intersection
			if ((inter_left < inter_right) && (inter_top < inter_bottom))
				return Rect<T>(inter_left, inter_top, inter_right - inter_left, inter_bottom - inter_top);

			return std::nullopt;
		}

		template<typename U>
		UC_NODISCARD constexpr Rect<U> cast() const
		{
			if constexpr (std::is_same_v<U, T>) return *this;

			return Rect<U>(pos.template cast<U>(), size.template cast<U>());
		}

		//constexpr static Rect<T>& clip(const Rect<T>& a, const Rect<T>& b, Rect<T>& result)
		//{
		//	result.x = Math::max(a.x, b.x);
		//	result.y = Math::max(a.y, b.y);
		//	return result;
		//}

		//constexpr static Rect<T> clip(const Rect<T>& a, const Rect<T>& b)
		//{
		//	Rect<T> result;
		//	clip(a, b, result);
		//	return result;
		//}

		Rect<T>& operator+=(const Vector2<T>& vec)
		{
			pos.x += vec.x;
			pos.y += vec.y;
			return *this;
		}

		Rect<T>& operator-=(const Vector2<T>& vec)
		{
			pos.x -= vec.x;
			pos.y -= vec.y;
			return *this;
		}

		static Vector2<T> normalized_to_point(const Rect& rect, const Vector2<T>& normalized)
		{
			return Vector2<T>(
				Math::lerp(rect.min_x(), rect.max_x(), normalized.x),
				Math::lerp(rect.min_y(), rect.max_y(), normalized.y)
				);
		}

		static Vector2<T> point_to_normalized(const Rect& rect, const Vector2<T>& point)
		{
			return Vector2<T>(
				Math::inverse_lerp(rect.min_x(), rect.max_x(), point.x),
				Math::inverse_lerp(rect.min_y(), rect.max_y(), point.y)
				);
		}

		static constexpr Rect<T> from_min_max(const Vector2<T>& min, const Vector2<T>& max);
		static constexpr Rect<T> from_center(const Vector2<T>& center, const Vector2<T>& size);
	};

	using Recti = Rect<int>;
	using Rectf = Rect<float>;

	// IMPLEMENTATION //////////////////////////////////////////////////////////
	template <typename T>
	constexpr Rect<T>::Rect(T x, T y, T w, T h)
		: pos(x, y), size(w, h)
	{
	}

	template <typename T>
	constexpr Rect<T>::Rect(T x, T y, const Vector2<T>& size)
		: pos(x, y), size(size)
	{
	}

	template <typename T>
	constexpr Rect<T>::Rect(const Vector2<T>& pos, T w, T h)
		: pos(pos), size(w, h)
	{
	}

	template <typename T>
	constexpr Rect<T>::Rect(const Vector2<T>& pos, const Vector2<T>& size)
		: pos(pos), size(size)
	{
	}

	template <typename T>
	constexpr Rect<T> Rect<T>::from_min_max(const Vector2<T>& min, const Vector2<T>& max)
	{
		return Rect<T>(min.x, min.y, max.x - min.x, max.y - min.y);
	}

	template <typename T>
	constexpr Rect<T> Rect<T>::from_center(const Vector2<T>& center, const Vector2<T>& size)
	{
		return Rect<T>(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
	}

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static constexpr bool operator == (const Rect<T>& a, const Rect<T>& b)
	{
		return
			Math::equals(a.pos, b.pos) && Math::equals(a.size, b.size);
	}

	template<typename T>
	static constexpr bool operator != (const Rect<T>& a, const Rect<T>& b)
	{
		return !(a == b);
	}

	template<typename T>
	static constexpr Rect<T> operator+ (const Rect<T>& rect, const Vector2<T>& vec)
	{
		return Rect<T>(rect.pos + vec, rect.size);
	}

	template<typename T>
	static constexpr Rect<T> operator- (const Rect<T>& rect, const Vector2<T>& vec)
	{
		return Rect<T>(rect.pos - vec, rect.size);
	}

	// CONST /////////////////////////////////////////////////////////////////////
	namespace details
	{
		template<typename T>
		struct RectConst
		{
			static constexpr Rect<T> Zero = Rect<T>(0, 0, 0, 0);
		};
	}

	using RectConsti = details::RectConst<int>;
	using RectConstf = details::RectConst<float>;

	template<typename T>
	extern UNICODE_STRING_BUILDER_FORMAT(const Rect<T>&)
	{
		return builder << value.pos << ';' << value.size;
	}

	UNICORE_MAKE_HASH(Recti)
	{
		return make(value.pos, value.size);
	}

	UNICORE_MAKE_HASH(Rectf)
	{
		return make(value.pos, value.size);
	}
}