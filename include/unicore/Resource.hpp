#pragma once
#include "unicore/Object.hpp"
#include "unicore/Stream.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	class Resource : public virtual Object
	{
	public:
	};

	enum class ResourceManipulatorCode
	{
		Success,
		FailedWrongFormat,
	};

	class ResourceManipulator : public Object
	{
	public:
		UC_NODISCARD virtual TypeIndex resource_type() const = 0;
	};
}