#pragma once
#include "unicore/Rect.hpp"

namespace unicore
{
	template<typename T>
	class IBuffer2
	{
	public:
		using FillFunction = std::function<T(int x, int y)>;
		using FillValue = Variant<T, FillFunction>;

		virtual ~IBuffer2() = default;

		UC_NODISCARD virtual const Vector2i& size() const = 0;

		UC_NODISCARD virtual bool get(int x, int y, T& value) const = 0;
		UC_NODISCARD virtual bool get(const Vector2i& pos, T& value) const { return get(pos.x, pos.y, value); }

		virtual bool set(int x, int y, T value) = 0;
		virtual bool set(const Vector2i& pos, T value) { return set(pos.x, pos.y, value); }

		virtual void fill(const FillValue& value)
		{
			const auto s = size();
			if (auto v = std::get_if<T>(&value))
			{
				for (auto y = 0; y < s.y; y++)
					for (auto x = 0; x < s.x; x++)
					{
						set(x, y, *v);
					}
			}
			else if (auto func = std::get_if<FillFunction>(&value))
			{
				for (auto y = 0; y < s.y; y++)
					for (auto x = 0; x < s.x; x++)
					{
						set(x, y, (*func)(x, y));
					}
			}
			else
			{
				UC_ASSERT_ALWAYS_MSG("Unimplemented");
			}
		}

		virtual void fill(const Recti& rect, const FillValue& value)
		{
			if (auto v = std::get_if<T>(&value))
			{
				for (auto y = rect.min_y(); y < rect.max_y(); y++)
					for (auto x = rect.min_x(); x < rect.max_x(); x++)
					{
						set(x, y, *v);
					}
			}
			else if (auto func = std::get_if<FillFunction>(&value))
			{
				for (auto y = rect.min_y(); y < rect.max_y(); y++)
					for (auto x = rect.min_x(); x < rect.max_x(); x++)
					{
						set(x, y, (*func)(x, y));
					}
			}
			else
			{
				UC_ASSERT_ALWAYS_MSG("Unimplemented");
			}
		}

		virtual void copy(const Recti& src_rect, IBuffer2<T>& dest, const Vector2i& offset = VectorConst2i::Zero) const
		{
			const auto s = dest.size();

			const Recti rect(
				offset.x, offset.y,
				Math::min(src_rect.w, s.x - offset.x),
				Math::min(src_rect.h, s.y - offset.y)
			);
			dest.fill(rect, [&](int x, int y) -> T
				{
					T value;
					return get(src_rect.x + x, src_rect.y + y, value) ? value : T{};
				});
		}

		virtual void copy(IBuffer2<T>& dest, const Vector2i& offset = VectorConst2i::Zero) const
		{
			const auto s = size();
			const Recti rect{ 0, 0, s.x, s.y };
			copy(rect, dest, offset);
		}
	};

	template<typename T>
	class Buffer2 : public IBuffer2<T>
	{
	public:
		explicit Buffer2(const Vector2i& size, T value)
			: _data(size.area(), value), _size(size)
		{}

		UC_NODISCARD const Vector2i& size() const override { return _size; }
		UC_NODISCARD bool get(int x, int y, T& value) const override
		{
			if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
			{
				auto offset = calc_offset(x, y);
				value = _data[offset];
				return true;
			}

			return false;
		}

		bool set(int x, int y, T value) override
		{
			if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
			{
				auto offset = calc_offset(x, y);
				_data[offset] = value;
				return true;
			}

			return false;
		}

		UC_NODISCARD constexpr int calc_offset(int x, int y) const { return y * _size.x + x; }
		UC_NODISCARD constexpr int calc_offset(const Vector2i& index) const { return calc_offset(index.x, index.y); }

		void resize(const Vector2i& size, const T& value)
		{
			_data.resize(size.area(), value);
		}

	protected:
		List<T> _data;
		Vector2i _size;
	};
}