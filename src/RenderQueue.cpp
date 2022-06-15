#include "unicore/RenderQueue.hpp"

namespace unicore
{
	RenderQueue::RenderQueue()
	{
		vertices.reserve(1024);
		commands.reserve(128);
	}

	void RenderQueue::begin()
	{
		vertices.clear();
		commands.clear();
		_cmd = {};
	}

	void RenderQueue::end()
	{
		flush();
	}

	void RenderQueue::flush()
	{
		if (_cmd.vcount > 0)
		{
			commands.push_back(_cmd);

			_cmd = {};
			_cmd.vstart = vertices.size();
			_cmd.vcount = 0;
		}
	}

	void RenderQueue::draw(Render& render) const
	{
		if (vertices.empty()) return;

		const auto data = vertices.data();

		for (const auto & cmd : commands)
		{
			render.set_state(cmd.state);
			render.draw_geometry(&data[cmd.vstart], cmd.vcount);
		}
	}

	RenderQueue& RenderQueue::set_primitive_type(PrimitiveType type)
	{
		if (_cmd.state.primitiveType != type)
		{
			flush();
			_cmd.state.primitiveType = type;
		}

		return *this;
	}

	RenderQueue& RenderQueue::set_clip(const Optional<Recti>& rect)
	{
		if (_cmd.state.clip_rect != rect)
		{
			flush();
			_cmd.state.clip_rect = rect;
		}

		return *this;
	}

	RenderQueue& RenderQueue::set_texture(const Shared<Texture>& texture)
	{
		if (_cmd.texture != texture)
		{
			flush();
			_cmd.texture = texture;
		}

		return *this;
	}

	RenderQueue& RenderQueue::add_vertex(const Vertex& v)
	{
		vertices.push_back(v);
		_cmd.vcount++;
		
		return *this;
	}

	RenderQueue& RenderQueue::add_vertices(const Vertex* data, size_t count)
	{
		//_vertices.insert(_vertices.end(), &data[0], &data[count]);

		for (auto i = 0; i < count; i++)
			vertices.push_back(data[i]);

		_cmd.vcount += count;
		return *this;
	}
}