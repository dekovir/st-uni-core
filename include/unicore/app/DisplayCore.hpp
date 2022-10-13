#pragma once
#include "unicore/app/Core.hpp"

namespace unicore
{
	class Display;

	struct DisplayCoreSettings : CoreSettings
	{
		DisplaySettings display;
	};

	class DisplayCore : public Core
	{
	public:
		explicit DisplayCore(const DisplayCoreSettings& settings);
		~DisplayCore() override;

		Display& display;

	protected:
		static DisplayCoreSettings create_settings(
			const CoreSettings& settings, StringView title, DisplayMode mode = { true });
	};
}