#pragma once
#include "unicore/Rect.hpp"

namespace unicore
{
	template<typename T>
	class IBuffer2;

	template<typename T>
	class IReadOnlyBuffer2
	{
	public:
		virtual ~IReadOnlyBuffer2() = default;

		UC_NODISCARD virtual const Vector2i& size() const = 0;

		UC_NODISCARD Recti get_rect() const
		{
			return { VectorConst2i::Zero, size() };
		}

		UC_NODISCARD virtual bool get(int x, int y, T& value) const = 0;
		UC_NODISCARD bool get(const Vector2i& pos, T& value) const { return get(pos.x, pos.y, value); }
	};

	template<typename T>
	class IBuffer2 : public IReadOnlyBuffer2<T>
	{
	public:
		virtual bool set(int x, int y, T value) = 0;
		bool set(const Vector2i& pos, T value) { return set(pos.x, pos.y, value); }

		virtual void clip_rect(Recti& rect) const
		{
			if (rect.x < 0)
			{
				rect.w += rect.x;
				rect.x = 0;
			}

			if (rect.y < 0)
			{
				rect.h += rect.y;
				rect.y = 0;
			}

			const auto s = this->size();
			if (rect.x + rect.w > s.x)
				rect.w = s.x - rect.x;

			if (rect.y + rect.h > s.y)
				rect.h = s.y - rect.y;
		}

		UC_NODISCARD virtual Recti clip_rect(const Recti& rect) const
		{
			Recti result(rect);
			clip_rect(result);
			return result;
		}
	};

	template<typename T>
	class Buffer2 : public IBuffer2<T>
	{
	public:
		explicit Buffer2(const Vector2i& size)
			: _data(size.area()), _size(size)
		{}

		Buffer2(int x, int y)
			: _data(x* y), _size(x, y)
		{}

		T* data() { return _data.data(); }
		const T* data() const { return _data.data(); }

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