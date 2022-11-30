#pragma once
#include "unicore/resource/Resource.hpp"
#include "unicore/system/TimeSpan.hpp"
#include "unicore/system/Event.hpp"
#include "unicore/scene/SceneComponent.hpp"

namespace unicore
{
	class Scene : public Resource
	{
		UC_OBJECT_EVENT(update, const TimeSpan&);
	public:
		UC_NODISCARD size_t get_system_memory_use() const override;

		void add_component(Unique<SceneComponent>& component);

		template<typename T,
			std::enable_if_t<std::is_base_of_v<SceneComponent, T>>* = nullptr>
		void create_component()
		{
			add_component(std::make_unique<T>());
		}

		UC_NODISCARD SceneComponent* get(const TypeInfo& type);
		UC_NODISCARD const SceneComponent* get(const TypeInfo& type) const;

		template<typename T,
			std::enable_if_t<std::is_base_of_v<SceneComponent, T>>* = nullptr>
		UC_NODISCARD SceneComponent* get()
		{
			return get(get_type_info<T>());
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<SceneComponent, T>>* = nullptr>
		UC_NODISCARD const SceneComponent* get() const
		{
			return get(get_type_info<T>());
		}

		void update(const TimeSpan& delta);

	protected:
		List<Unique<SceneComponent>> _components;
		Dictionary<TypeIndex, SceneComponent*> _type_dict;
	};
}