#pragma once
#include "unicore/Object.hpp"
#include "unicore/EnumFlag.hpp"
#include "unicore/Event.hpp"
#include "unicore/Vector2.hpp"

namespace unicore
{
	class Logger;

	enum class DisplayFlag : uint8_t
	{
		Resizable = 1 << 0,
		Borderless = 1 << 1,
		Fullscreen = 1 << 2,
	};
	UNICORE_ENUMFLAGS(DisplayFlag, DisplayFlags);

	struct DisplaySettings
	{
		Logger& logger;
		StringView title;
		Vector2i size;
		DisplayFlags flags;
	};

	class Display : public Object
	{
		UC_OBJECT(Display, Object)
			UC_OBJECT_EVENT(resize, const Vector2i&)
	public:
		explicit Display(const DisplaySettings& settings);

		UC_NODISCARD virtual const Vector2i& size() const = 0;
		UC_NODISCARD virtual void* native_handle() const = 0;

	protected:
		Logger& _logger;
	};
}