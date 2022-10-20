#pragma once
#include "unicore/system/Object.hpp"
#include "unicore/system/Buffer2.hpp"

namespace unicore
{
	class RaycastRenderer : public Object
	{
	public:
		virtual void render() const = 0;
	};
}