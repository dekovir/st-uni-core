#include "unicore/scene/Scene.hpp"

namespace unicore
{
	size_t Scene::get_system_memory_use() const
	{
		// TODO: Calc used memory size
		return 0;
	}

	void Scene::add_component(Unique<SceneComponent>& component)
	{
		_type_dict[get_type_index(component)] = component.get();
		_components.push_back(std::move(component));
	}

	SceneComponent* Scene::get(const TypeInfo& type)
	{
		const auto it = _type_dict.find(type);
		return it != _type_dict.end() ? it->second : nullptr;
	}

	const SceneComponent* Scene::get(const TypeInfo& type) const
	{
		const auto it = _type_dict.find(type);
		return it != _type_dict.end() ? it->second : nullptr;
	}

	void Scene::update(const TimeSpan& delta)
	{
		_event_update.invoke(delta);
	}
}