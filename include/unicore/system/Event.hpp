#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename ... Args>
	class PublicEvent
	{
	public:
		using ActionType = StdVariant<std::function<void()>, Action<Args...>>;

		UC_NODISCARD bool empty() const { return _actions.empty(); }

		void add(const ActionType& action)
		{
			_actions.push_back(action);
		}

		bool remove(const ActionType& action)
		{
			// TODO: Fix compile error
			//for (auto it = _actions.begin(); it != _actions.end(); ++it)
			//{
			//	if (*it == action)
			//	{
			//		_actions.erase(it);
			//		return true;
			//	}
			//}

			return false;
		}

		PublicEvent& operator+=(const ActionType& action)
		{
			add(action);
			return *this;
		}

		PublicEvent& operator-=(const ActionType& action)
		{
			remove(action);
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
			{
				if (auto ptr = std::get_if<Action<Args...>>(&action))
				{
					(*ptr)(std::forward<Args>(args)...);
				}
				else if (auto ptr = std::get_if<std::function<void()>>(&action))
				{
					(*ptr)();
				}
			}
		}

		void operator()(Args... args)
		{
			invoke(std::forward<Args>(args)...);
		}
	};

#define UC_OBJECT_EVENT(name, ...) \
	public: using Event_ ## name = PublicEvent<__VA_ARGS__>; \
	public: PublicEvent<__VA_ARGS__>& on_ ## name() { return _event_ ## name; } \
	protected: Event<__VA_ARGS__> _event_ ## name
}