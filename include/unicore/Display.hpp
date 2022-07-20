#pragma once
#include "unicore/Object.hpp"
#include "unicore/EnumFlag.hpp"
#include "unicore/Vector2.hpp"

namespace unicore
{
	class Logger;

	enum class DisplayState : uint8_t
	{
		Visible = 1 << 0,
		Focused = 1 << 1,
	};
	UNICORE_ENUMFLAGS(DisplayState, DisplayStateFlags);

	enum class DisplayWindowFlag : uint8_t
	{
		Resizable = 1 << 0,
		Borderless = 1 << 1,
	};
	UNICORE_ENUMFLAGS(DisplayWindowFlag, DisplayWindowFlags);

	enum class DisplayMode
	{
		Window,
		Fullscreen,
	};

	struct DisplaySettings
	{
		Logger& logger;
		StringView title;
		Vector2i size;
		DisplayMode mode = DisplayMode::Fullscreen;
		DisplayWindowFlags window_flags = DisplayWindowFlags::Zero;
	};

	class Display : public Object
	{
		UC_OBJECT(Display, Object)
	public:
		explicit Display(const DisplaySettings& settings);

		UC_NODISCARD virtual const Vector2i& size() const = 0;
		UC_NODISCARD virtual DisplayMode mode() const = 0;
		UC_NODISCARD virtual DisplayWindowFlags window_flags() const = 0;

		virtual void set_windowed(const Vector2i& size, DisplayWindowFlags window_flags = DisplayWindowFlags::Zero) = 0;
		virtual void set_fullscreen() = 0;

		UC_NODISCARD virtual void* native_handle() const = 0;

	protected:
		Logger& _logger;
	};
}