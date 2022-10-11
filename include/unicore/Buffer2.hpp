#pragma once
#include "unicore/Rect.hpp"

namespace unicore
{
	template<typename T>
	class IReadOnlyBuffer2
	{
	public:
		virtual ~IReadOnlyBuffer2() = default;

		//UC_NODISCARD virtual const Vector2i& start() const = 0;
		UC_NODISCARD virtual const Vector2i& size() const = 0;

		UC_NODISCARD virtual Recti rect() const
		{
			return { VectorConst2i::Zero, size() };
		}

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

		UC_NODISCARD virtual Recti get_clip_rect(const Recti& rect) const
		{
			Recti result(rect);
			clip_rect(result);
			return result;
		}

		UC_NODISCARD virtual bool get(int x, int y, T& value) const = 0;
		UC_NODISCARD bool get(const Vector2i& pos, T& value) const { return get(pos.x, pos.y, value); }

		UC_NODISCARD T get_or_default(int x, int y, const T& default_value) const
		{
			T value;
			return get(x, y, value) ? value : default_value;
		}

		UC_NODISCARD T get_or_default(const Vector2i& pos, const T& default_value) const
		{
			return get_or_default(pos.x, pos.y, default_value);
		}

		// TODO: Replace raw pointer
		virtual unsigned get_line_h(int x, int y, unsigned lng, T* dest) const
		{
			T value;
			for (unsigned i = 0; i < lng; i++)
			{
				if (get(x + i, y, value))
					dest[i] = value;
				else return i;
			}

			return lng;
		}

		virtual unsigned get_line_h(const Vector2i& pos, unsigned lng, T* dest) const
		{
			return get_line_h(pos.x, pos.y, lng, dest);
		}

		// TODO: Replace raw pointer
		virtual unsigned get_line_v(int x, int y, unsigned lng, T* dest) const
		{
			T value;
			for (unsigned i = 0; i < lng; i++)
			{
				if (get(x, y + i, value))
					dest[i] = value;
				else return i;
			}

			return lng;
		}

		virtual unsigned get_line_v(const Vector2i& pos, unsigned lng, T* dest) const
		{
			return get_line_v(pos.x, pos.y, lng, dest);
		}
	};

	template<typename T>
	class IBuffer2 : public IReadOnlyBuffer2<T>
	{
	public:
		virtual bool set(int x, int y, T value) = 0;
		bool set(const Vector2i& pos, T value) { return set(pos.x, pos.y, value); }

		// TODO: Replace raw pointer
		virtual unsigned set_line_h(int x, int y, unsigned lng, const T& value)
		{
			for (unsigned i = 0; i < lng; i++)
			{
				if (!set(x + i, y, value))
					return i;
			}
			return lng;
		}

		virtual unsigned set_line_h(const Vector2i& pos, unsigned lng, const T& value)
		{
			return set_line_h(pos.x, pos.y, lng, value);
		}

		// TODO: Replace raw pointer
		virtual unsigned set_line_v(int x, int y, unsigned lng, const T& value)
		{
			for (unsigned i = 0; i < lng; i++)
			{
				if (!set(x, y + i, value))
					return i;
			}
			return lng;
		}

		virtual unsigned set_line_v(const Vector2i& pos, unsigned lng, const T& value)
		{
			return set_line_v(pos.x, pos.y, lng, value);
		}

		// TODO: Replace raw pointer
		virtual unsigned set_line_h(int x, int y, unsigned lng, const T* src)
		{
			for (unsigned i = 0; i < lng; i++)
			{
				if (!set(x + i, y, src[i]))
					return i;
			}
			return lng;
		}

		virtual unsigned set_line_h(const Vector2i& pos, unsigned lng, const T* src)
		{
			return set_line_h(pos.x, pos.y, lng, src);
		}

		// TODO: Replace raw pointer
		virtual unsigned set_line_v(int x, int y, unsigned lng, const T* src)
		{
			for (unsigned i = 0; i < lng; i++)
			{
				if (!set(x, y + i, src[i]))
					return i;
			}
			return lng;
		}

		virtual unsigned set_line_v(const Vector2i& pos, unsigned lng, const T* src)
		{
			return set_line_v(pos.x, pos.y, lng, src);
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

		unsigned get_line_h(int x_, int y_, unsigned lng_, T* dest) const override
		{
			const auto x = Math::clamp(x_, 0, _size.x);
			const auto y = Math::clamp(y_, 0, _size.y);
			const auto lng = Math::min<unsigned>(lng_, _size.x - x);

			const auto offset = calc_offset(x, y);
			for (unsigned i = 0; i < lng; i++)
				dest[i] = _data[offset + i];
			return lng;
		}

		unsigned get_line_v(int x_, int y_, unsigned lng_, T* dest) const override
		{
			const auto x = Math::clamp(x_, 0, _size.x);
			const auto y = Math::clamp(y_, 0, _size.y);
			const auto lng = Math::min<unsigned>(lng_, _size.y - y);

			const auto offset = calc_offset(x, y);
			for (unsigned i = 0; i < lng; i++)
				dest[i] = _data[offset + _size.x];
			return lng;
		}

		unsigned set_line_h(int x_, int y_, unsigned lng_, const T& value) override
		{
			const auto x = Math::clamp(x_, 0, _size.x);
			const auto y = Math::clamp(y_, 0, _size.y);
			const auto lng = Math::min<unsigned>(lng_, _size.x - x);

			const auto offset = calc_offset(x, y);
			for (unsigned i = 0; i < lng; i++)
				_data[offset + i] = value;
			return lng;
		}

		unsigned set_line_v(int x_, int y_, unsigned lng_, const T& value) override
		{
			const auto x = Math::clamp(x_, 0, _size.x);
			const auto y = Math::clamp(y_, 0, _size.y);
			const auto lng = Math::min<unsigned>(lng_, _size.y - y);

			const auto offset = calc_offset(x, y);
			for (unsigned i = 0; i < lng; i++)
				_data[offset + _size.x] = value;
			return lng;
		}

		unsigned set_line_h(int x_, int y_, unsigned lng_, const T* src) override
		{
			const auto x = Math::clamp(x_, 0, _size.x);
			const auto y = Math::clamp(y_, 0, _size.y);
			const auto lng = Math::min<unsigned>(lng_, _size.x - x);

			const auto offset = calc_offset(x, y);
			for (unsigned i = 0; i < lng; i++)
				_data[offset + i] = src[i];
			return lng;
		}

		unsigned set_line_v(int x_, int y_, unsigned lng_, const T* src) override
		{
			const auto x = Math::clamp(x_, 0, _size.x);
			const auto y = Math::clamp(y_, 0, _size.y);
			const auto lng = Math::min<unsigned>(lng_, _size.y - y);

			const auto offset = calc_offset(x, y);
			for (unsigned i = 0; i < lng; i++)
				_data[offset + _size.x] = src[i];
			return lng;
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