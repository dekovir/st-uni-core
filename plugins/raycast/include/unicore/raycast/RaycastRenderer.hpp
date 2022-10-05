#pragma once
#include "unicore/Object.hpp"
#include "unicore/Buffer2.hpp"

namespace unicore
{
	class RaycastRenderer : public Object
	{
	public:
		virtual void render() const = 0;
	};
}