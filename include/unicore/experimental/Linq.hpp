#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	namespace linq
	{
		template<typename T, typename TIterator>
		class IteratorRange
		{
		public:
			constexpr IteratorRange(TIterator begin, TIterator end)
				: _begin(begin), _end(end)
			{}

			UC_NODISCARD bool any(const Predicate<T>& predicate) const
			{
				return std::any_of(_begin, _end, predicate);
			}

			UC_NODISCARD bool all(const Predicate<T>& predicate) const
			{
				return std::all_of(_begin, _end, predicate);
			}

			UC_NODISCARD bool none(const Predicate<T>& predicate) const
			{
				return std::none_of(_begin, _end, predicate);
			}

			UC_NODISCARD TIterator find(const Predicate<T>& predicate) const
			{
				return std::find_if(_begin, _end, predicate);
			}

		protected:
			TIterator _begin, _end;
		};

		template<typename T>
		static auto from(const List<T>& list)
		{
			return IteratorRange<T, typename List<T>::const_iterator>(list.begin(), list.end());
		}

		template<typename T>
		static auto from_reverse(const List<T>& list)
		{
			return IteratorRange<T, typename List<T>::const_iterator>(list.rbegin(), list.rend());
		}
	}
}