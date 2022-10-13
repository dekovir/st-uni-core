#include "unicore/app/DisplayCore.hpp"
#include "unicore/platform/Input.hpp"

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
		const CoreSettings& settings, StringView title, DisplayMode mode)
	{
		static ProxyLogger logger("[Display] ", settings.platform.logger);

		return {
			settings,
			{
				logger,
				title,
				mode
			}
		};
	}
}