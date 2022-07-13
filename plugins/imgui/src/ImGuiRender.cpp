#include "unicore/imgui/ImGuiRender.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	ImGuiRender::ImGuiRender(Logger& logger)
		: _logger(logger)
	{
	}

	void ImGuiRender::init(Renderer& render)
	{
		const auto& io = ImGui::GetIO();

		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		Surface surface(width, height);
		Memory::copy(surface.data(), pixels, surface.data_size());

		_font_texture = render.create_texture(surface);
		if (_font_texture)
		{
			io.Fonts->SetTexID(_font_texture.get());
		}
		else
		{
			UC_LOG_ERROR(_logger) << "Failed to create font texture";
		}
	}

	ImGuiRender2D::ImGuiRender2D(Renderer2D& render, Logger& logger)
		: ImGuiRender(logger), _render(render)
	{
	}

	void ImGuiRender2D::begin_frame(ImGui::IO& io)
	{
		const auto size = _render.screen_size();
		io.DisplaySize.x = static_cast<float>(size.x);
		io.DisplaySize.y = static_cast<float>(size.y);

		io.DisplayFramebufferScale.x = 1;
		io.DisplayFramebufferScale.y = 1;
	}

	void ImGuiRender2D::render(const ImDrawData* draw_data)
	{
		// If there's a scale factor set by the user, use that instead
		// If the user has specified a scale factor to SDL_Renderer already via SDL_RenderSetScale(), SDL will scale whatever we pass
		// to SDL_RenderGeometryRaw() by that scale factor. In that case we don't want to be also scaling it ourselves here.
		float rsx = 1.0f;
		float rsy = 1.0f;
		//SDL_RenderGetScale(bd->SDLRenderer, &rsx, &rsy);
		ImVec2 render_scale;
		render_scale.x = (rsx == 1.0f) ? draw_data->FramebufferScale.x : 1.0f;
		render_scale.y = (rsy == 1.0f) ? draw_data->FramebufferScale.y : 1.0f;

		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		int fb_width = (int)(draw_data->DisplaySize.x * render_scale.x);
		int fb_height = (int)(draw_data->DisplaySize.y * render_scale.y);
		if (fb_width == 0 || fb_height == 0)
			return;

		// Backup SDL_Renderer state that will be modified to restore it afterwards
		struct BackupSDLRendererState
		{
			Recti Viewport;
			Optional<Recti> ClipRect;
		};
		BackupSDLRendererState old = {};

		old.ClipRect = _render.get_clip();
		//SDL_RenderGetViewport(bd->SDLRenderer, &old.Viewport);

		// Will project scissor/clipping rectangles into framebuffer space
		ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
		ImVec2 clip_scale = render_scale;

		// Render command lists

		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
			const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					// User callback, registered via ImDrawList::AddCallback()
					// (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
					if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
						setup_render_state();
					else
						pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					// Project scissor/clipping rectangles into framebuffer space
					ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
					ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
					if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
					if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
					if (clip_max.x > fb_width) { clip_max.x = (float)fb_width; }
					if (clip_max.y > fb_height) { clip_max.y = (float)fb_height; }
					if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
						continue;

					Recti r = { (int)(clip_min.x), (int)(clip_min.y), (int)(clip_max.x - clip_min.x), (int)(clip_max.y - clip_min.y) };

					_render.set_clip(r);

					static List<VertexTexColor2> s_verts;

					s_verts.resize(pcmd->ElemCount);
					for (unsigned i = 0; i < pcmd->ElemCount; i++)
					{
						const auto index = idx_buffer[pcmd->IdxOffset + i];
						const auto& [pos, uv, col] = vtx_buffer[pcmd->VtxOffset + index];

						VertexTexColor2 vertex;
						vertex.pos.x = pos.x;
						vertex.pos.y = pos.y;
						vertex.uv.x = uv.x;
						vertex.uv.y = uv.y;
						vertex.col = Color4b::from_format(pixel_format_abgr, col);
						s_verts[i] = vertex;
					}

					const auto tex = static_cast<Texture*>(pcmd->GetTexID());
					_render.draw_triangles(s_verts.data(), s_verts.size(), tex);
				}
			}
		}

		// Restore modified SDL_Renderer state
		//SDL_RenderSetViewport(bd->SDLRenderer, &old.Viewport);
		_render.set_clip(old.ClipRect);
	}

	void ImGuiRender2D::setup_render_state()
	{
		//SDL_RenderSetViewport(bd->SDLRenderer, NULL);
		_render.set_clip(std::nullopt);
	}
}