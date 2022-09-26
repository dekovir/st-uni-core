#pragma once
#include "unicore/Canvas.hpp"

namespace unicore
{
	template<typename T>
	class IBuffer2;

	template<typename T>
	class IReadOnlyBuffer2
	{
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;

		UC_NODISCARD virtual bool get(int x, int y, T& value) const = 0;
		UC_NODISCARD virtual bool get(const Vector2i& pos, T& value) const { return get(pos.x, pos.y, value); }

		virtual void copy(const Recti& src_rect, IBuffer2<T>& dest, const Vector2i& offset = VectorConst2i::Zero) const
		{
			const auto s = dest.size();

			const Recti rect(
				offset.x, offset.y,
				Math::min(src_rect.w, s.x - offset.x),
				Math::min(src_rect.h, s.y - offset.y)
			);
			dest.fill([&](int x, int y) -> T
				{
					T value;
					return get(src_rect.x + x, src_rect.y + y, value) ? value : T{};
				}, rect);
		}

		virtual void copy(IBuffer2<T>& dest, const Vector2i& offset = VectorConst2i::Zero) const
		{
			const auto s = size();
			const Recti rect{ 0, 0, s.x, s.y };
			copy(rect, dest, offset);
		}
	};

	template<typename T>
	class IBuffer2 : public IReadOnlyBuffer2<T>
	{
	public:
		using FillFunction = std::function<T(int x, int y)>;

		virtual ~IBuffer2() = default;

		virtual bool set(int x, int y, T value) = 0;
		virtual bool set(const Vector2i& pos, T value) { return set(pos.x, pos.y, value); }

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

		virtual void fill(const T& value, const Optional<Recti>& rect = std::nullopt)
		{
			const auto s = this->size();
			if (rect.has_value())
			{
				const auto r = clip_rect(rect.value());
				for (auto y = r.min_y(); y < r.max_y(); ++y)
					for (auto x = r.min_x(); x < r.max_x(); ++x)
						set(x, y, value);
			}
			else
			{
				for (auto y = 0; y < s.y; y++)
					for (auto x = 0; x < s.x; x++)
					{
						set(x, y, value);
					}
			}
		}

		virtual void fill(FillFunction func, const Optional<Recti>& rect = std::nullopt)
		{
			const auto s = this->size();
			if (rect.has_value())
			{
				const auto r = clip_rect(rect.value());
				for (auto y = r.min_y(); y < r.max_y(); ++y)
					for (auto x = r.min_x(); x < r.max_x(); ++x)
						set(x, y, func(x, y));
			}
			else
			{
				for (auto y = 0; y < s.y; y++)
					for (auto x = 0; x < s.x; x++)
						set(x, y, func(x, y));
			}
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

	template<typename T>
	struct BufferSet
	{
		void operator()(IBuffer2<T>& buffer, int x, int y, const T& value) const
		{
			buffer.set(x, y, value);
		}
	};

	template<typename T>
	using BufferGetAction = Action<int, int, const T&>;

	template<typename T>
	struct BufferGet
	{
		void operator()(IBuffer2<T>& buffer, int x, int y, const BufferGetAction<T>& callback) const
		{
			T value;
			if (buffer.get(x, y, value))
				callback(x, y, value);
		}
	};

	template<typename TValue>
	using BufferCanvas = Canvas<IBuffer2<TValue>, TValue, BufferSet<TValue>>;

	template<typename TValue>
	using BufferCanvasGet = Canvas<IBuffer2<TValue>, BufferGetAction<TValue>, BufferGet<TValue>>;
}