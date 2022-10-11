#include "InitPlugins.hpp"

#if defined(UNICORE_USE_FNT)
#include "unicore/fnt/FNTPlugin.hpp"
#endif

#if defined(UNICORE_USE_GRID)
#include "unicore/grid/GridPlugin.hpp"
#endif

#if defined(UNICORE_USE_IMGUI)
#include "unicore/imgui/ImGuiPlugin.hpp"
#endif

#if defined(UNICORE_USE_STB)
#include "unicore/stb/StbPlugin.hpp"
#endif

#if defined(UNICORE_USE_7ZIP)
#include "unicore/szip/SZipFilePlugin.hpp"
#endif

#if defined(UNICORE_USE_WASM)
#include "unicore/wasm/WasmPlugin.hpp"
#endif

#if defined(UNICORE_USE_XML)
#include "unicore/xml/XMLPlugin.hpp"
#endif

namespace unicore
{
	void init_plugins(Core& core)
	{
#if defined(UNICORE_USE_FNT)
		core.create_plugin<FNTPlugin>();
#endif

#if defined(UNICORE_USE_GRID)
		core.create_plugin<GridPlugin>();
#endif

#if defined(UNICORE_USE_IMGUI)
		core.create_plugin<ImGuiPlugin>();
#endif

#if defined(UNICORE_USE_STB)
		core.create_plugin<StbPlugin>();
#endif

#if defined(UNICORE_USE_7ZIP)
		core.create_plugin<SZipFilePlugin>();
#endif

#if defined(UNICORE_USE_WASM)
		core.create_plugin<WasmPlugin>();
#endif

#if defined(UNICORE_USE_XML)
		core.create_plugin<XMLPlugin>();
#endif
	}
}