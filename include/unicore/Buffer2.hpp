#pragma once
#include "unicore/Rect.hpp"

namespace unicore
{
	template<typename T>
	class IBuffer2
	{
	public:
		virtual ~IBuffer2() = default;

		UC_NODISCARD virtual const Vector2i& size() const = 0;

		UC_NODISCARD virtual bool get(int x, int y, T& value) const = 0;
		UC_NODISCARD virtual bool get(const Vector2i& pos, T& value) const { return get(pos.x, pos.y, value); }

		virtual bool set(int x, int y, T value) = 0;
		virtual bool set(const Vector2i& pos, T value) { return set(pos.x, pos.y, value); }
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

	namespace BufferDraw2
	{
		template<typename T>
		extern void fill(IBuffer2<T>& buffer, T value)
		{
			auto& size = buffer.size();
			for (auto y = 0; y < size.y; y++)
				for (auto x = 0; x < size.x; x++)
					buffer.set(x, y, value);
		}

		template<typename T>
		extern void fill(IBuffer2<T>& buffer, const Recti& rect, T value)
		{
			for (auto y = rect.min_y(); y < rect.max_y(); y++)
				for (auto x = rect.min_x(); x < rect.max_x(); x++)
					buffer.set(x, y, value);
		}

		template<typename T>
		extern void fill(IBuffer2<T>& buffer, std::function<T(int x, int y)> func)
		{
			auto& size = buffer.size();
			for (auto y = 0; y < size.y; y++)
				for (auto x = 0; x < size.x; x++)
				{
					buffer.set(x, y, func(x, y));
				}
		}

		template<typename T>
		extern void fill(IBuffer2<T>& buffer, const Recti& rect, std::function<T(int x, int y)> func)
		{
			for (auto y = rect.min_y(); y < rect.max_y(); y++)
				for (auto x = rect.min_x(); x < rect.max_x(); x++)
					buffer.set(x, y, func(x, y));
		}
	}
}