#pragma once
#include "unicore/ui/components/Component.hpp"

namespace unicore::ui
{
	class NodeComponent : public Component
	{
	public:
		explicit NodeComponent(UINodeTag tag);

		UC_NODISCARD UINodeTag tag() const { return _tag; }
		UC_NODISCARD const UINode& node() const { return _node; }

	protected:
		UC_NODISCARD Variant get_attribute(UIAttribute type) const { return _node.get(type); }

		void set_attribute(UIAttribute type, const Variant& value);
		void set_action(UIActionType type, const UIAction& action);

		void on_mount() override;
		void on_dismount() override;

	private:
		const UINodeTag _tag;
		UINodeOptions _options;

		UINode _node = UINode::Empty;
	};
}