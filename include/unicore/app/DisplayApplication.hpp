#pragma once
#include "unicore/app/Application.hpp"

namespace unicore
{
	class Display;

	struct DisplayCoreSettings : CoreSettings
	{
		DisplaySettings display;
	};

	class DisplayApplication : public Application
	{
	public:
		explicit DisplayApplication(const DisplayCoreSettings& settings);
		~DisplayApplication() override;

		Display& display;

	protected:
		static DisplayCoreSettings create_settings(
			const CoreSettings& settings, StringView title, DisplayMode mode = { true });
	};
}