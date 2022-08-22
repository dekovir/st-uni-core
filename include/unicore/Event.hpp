#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename ... Args>
	class PublicEvent
	{
	public:
		using ActionType = Action<Args...>;

		void add(const ActionType& action)
		{
			_actions.push_back(action);
		}

		PublicEvent& operator+=(const ActionType& action)
		{
			add(action);
			return *this;
		}

	protected:
		List<ActionType> _actions;
	};

	template<typename ... Args>
	class Event : public PublicEvent<Args...>
	{
	public:

		void invoke(Args... args)
		{
			for (const auto& action : PublicEvent<Args...>::_actions)
				action(std::forward<Args>(args)...);
		}

		void operator()(Args... args)
		{
			invoke(std::forward<Args>(args)...);
		}
	};

#define UC_OBJECT_EVENT(name, ...) \
	public:  PublicEvent<__VA_ARGS__>& on_ ## name() { return _event_ ## name; } \
	protected: Event<__VA_ARGS__> _event_ ## name
}