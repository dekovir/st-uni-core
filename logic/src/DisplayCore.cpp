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

	DisplayCoreSettings DisplayCore::create_settings(
		const CoreSettings& settings, StringView title, const Vector2i& size, DisplayFlags flags)
	{
		static ProxyLogger logger("[Display] ", settings.platform.logger);

		return {
			settings,
			{
				logger,
				title,
				size.x <= 0 || size.y <= 0 ? settings.platform.native_size() : size,
				flags
			}
		};
	}
}