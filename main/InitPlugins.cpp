#include "InitPlugins.hpp"
#include "unicore/resource/ResourcePlugin.hpp"
#include "unicore/renderer/SurfacePlugin.hpp"
#include "unicore/renderer/RendererPlugin.hpp"

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

#if defined(UNICORE_USE_REMOTEUI)
#include "unicore/remoteui/RemoteUIPlugin.hpp"
#endif

#if defined(UNICORE_USE_SCENE)
#include "unicore/scene/ScenePlugin.hpp"
#endif

namespace unicore
{
	void init_plugins(Application& app)
	{
		app.create_plugin<ResourcePlugin>();
		app.create_plugin<SurfacePlugin>();
		app.create_plugin<RendererPlugin>();

#if defined(UNICORE_USE_FNT)
		app.create_plugin<FNTPlugin>();
#endif

#if defined(UNICORE_USE_GRID)
		app.create_plugin<GridPlugin>();
#endif

#if defined(UNICORE_USE_IMGUI)
		app.create_plugin<ImGuiPlugin>();
#endif

#if defined(UNICORE_USE_STB)
		app.create_plugin<StbPlugin>();
#endif

#if defined(UNICORE_USE_7ZIP)
		app.create_plugin<SZipFilePlugin>();
#endif

#if defined(UNICORE_USE_WASM)
		app.create_plugin<WasmPlugin>();
#endif

#if defined(UNICORE_USE_XML)
		app.create_plugin<XMLPlugin>();
#endif

#if defined(UNICORE_USE_REMOTEUI)
		app.create_plugin<remoteui::RemoteUIPlugin>();
#endif

#if defined(UNICORE_USE_SCENE)
		app.create_plugin<ScenePlugin>();
#endif
	}
}