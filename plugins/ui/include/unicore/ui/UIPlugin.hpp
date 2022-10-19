#pragma once
#include "unicore/system/Event.hpp"
#include "unicore/math/Rect.hpp"

namespace unicore
{
	using UIElementUID = UInt32;

	using UICallback = std::function<void()>;

	using UIElementValue = Variant<Bool, Int64, Double, String32, UICallback>;

	enum class UIElementType
	{
		Div,
		Text,
		Button,
	};

	struct UIElementData
	{
		UIElementType type;
		Dictionary<StringView, UIElementValue> values;

		static constexpr StringView Text = "text";
		static constexpr StringView Action = "action";

		static UIElementData make_div()
		{
			return
			{
				UIElementType::Div,
			};
		}

		static UIElementData make_label(StringView32 text)
		{
			return
			{
				UIElementType::Text,
				{
					{Text, String32(text)}
				}
			};
		}

		static UIElementData make_button(StringView32 text, const UICallback& action)
		{
			return
			{
				UIElementType::Button,
				{
					{Text, String32(text)},
					{Action, action},
				}
			};
		}
	};

	struct UIElement;
	using UIElementList = List<UIElement>;

	struct UIElement
	{
		UIElementUID uid;
		UIElementData data;
		UIElementList childs;
	};

	enum class EventType
	{
		Rebuild,
		ChangeData,
		ChangeState,
		RemoveElement,
	};

	struct UIEvent
	{
		EventType type;
	};

	class UIBackend
	{
		UC_OBJECT_EVENT(rebuild, const UIElementList&);
	public:
		virtual ~UIBackend() = default;
	};

	class UIFrontend
	{
	public:
		UIFrontend()
			: _bind_rebuild([this](auto& elements) { on_rebuild(elements); })
		{
		}

		virtual ~UIFrontend() = default;

		UC_NODISCARD const Shared<UIBackend>& backend() const { return _backend; }

		void set_backend(const Shared<UIBackend>& backend)
		{
			if (_backend == backend)
				return;

			if (_backend)
			{
				_backend->on_rebuild() -= _bind_rebuild;
			}

			_backend = backend;

			if (_backend)
			{
				_backend->on_rebuild() += _bind_rebuild;
			}
		}

	protected:
		const Action<const UIElementList&>& _bind_rebuild;

		Shared<UIBackend> _backend;

		void on_rebuild(const UIElementList& elements)
		{
		}
	};
}

namespace unicore
{
	class UIValue
	{
	public:
		virtual ~UIValue() = default;
		UC_NODISCARD virtual const TypeInfo& type_info() const = 0;
	};

	template<typename T>
	class UIValueTyped : public UIValue
	{
	public:
		UC_NODISCARD const TypeInfo& type_info() const override { return get_type_info<T>(); }
	};

	class UICollectionValue : public UIValue
	{
	public:
		UC_NODISCARD virtual const TypeInfo& element_type() const = 0;
		UC_NODISCARD virtual size_t size() const = 0;
	};

	template<typename TElement>
	class UIListValue : public UICollectionValue
	{
	public:
		using CollectionType = List<TElement>;

		explicit UIListValue(CollectionType& collection)
			: _collection(collection)
		{
		}

		UC_NODISCARD const TypeInfo& type_info() const override { return get_type_info<CollectionType>(); }
		UC_NODISCARD const TypeInfo& element_type() const override { return get_type_info<TElement>(); }
		UC_NODISCARD size_t size() const override { return _collection.size(); }

	protected:
		CollectionType& _collection;
	};
}