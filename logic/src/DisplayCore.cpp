#include "unicore/app/DisplayCore.hpp"

namespace unicore
{
	namespace internal
	{
		Unique<Display> display;

		static Display& create_display(Platform& platform, const DisplaySettings& settings)
		{
			if (!display)
				display = platform.create_display(settings);
			return *display;
		}
	}

	DisplayCore::DisplayCore(const DisplayCoreSettings& settings)
		: Core(settings)
		, display(internal::create_display(platform, settings.display))
	{
	}

	DisplayCore::~DisplayCore()
	{
		internal::display = nullptr;
	}

	DisplaySettings DisplayCore::create_settings(
		Platform& platform, StringView title, const Vector2i& size, DisplayFlags flags)
	{
		static ProxyLogger logger("[Display] ", platform.logger);

		return {
			logger,
			title,
			size.x <= 0 || size.y <= 0 ? platform.native_size() : size,
			flags
		};
	}
}