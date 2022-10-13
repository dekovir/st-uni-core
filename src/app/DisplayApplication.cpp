#include "unicore/app/DisplayApplication.hpp"
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

	DisplayApplication::DisplayApplication(const DisplayCoreSettings& settings)
		: Application(settings)
		, display(internal::create_display(platform, settings.display))
	{
	}

	DisplayApplication::~DisplayApplication()
	{
		internal::display = nullptr;
	}

	DisplayCoreSettings DisplayApplication::create_settings(
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