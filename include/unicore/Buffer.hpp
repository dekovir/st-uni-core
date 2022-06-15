#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/Vector3.hpp"
#include "unicore/Color.hpp"

namespace unicore
{
	template<typename T>
	class Buffer
	{
	public:
		explicit Buffer(size_t size, const T& value)
			: _data(size, value) {}

		[[nodiscard]] constexpr size_t size() const { return _data.size(); }

		void resize(size_t size, const T& value)
		{
			_data.resize(size, value);
		}

		constexpr T& operator[](int index) { return _data[index]; }
		constexpr const T& operator[](int index) const { return _data[index]; }

	protected:
		List<T> _data;
	};

	template<typename T>
	class Buffer2 : public Buffer<T>
	{
	public:
		explicit Buffer2(const Vector2i& size, const T& value)
			: Buffer(size.area(), value) {}

		[[nodiscard]] constexpr size_t count() const { return _data.size(); }
		[[nodiscard]] constexpr Vector2i& size() const { return _size; }

		[[nodiscard]] constexpr int CalcOffset(const Vector2i& index) const { return index.y * _size.x + index.x; }

		void resize(const Vector2i& size, const T& value)
		{
			_data.resize(size.area(), value);
		}

		constexpr T& operator[](const Vector2i& index) { return _data[CalcOffset(index)]; }
		constexpr const T& operator[](const Vector2i& index) const { return _data[CalcOffset(index)]; }

	protected:
		Vector2i _size;
	};

	template<typename T>
	class Buffer3 : public Buffer<T>
	{
	public:
		[[nodiscard]] const Vector3i& size() const { return _size; }

	protected:
		Vector3i _size;
	};

	using ColorBuffer = Buffer2<Color>;
}