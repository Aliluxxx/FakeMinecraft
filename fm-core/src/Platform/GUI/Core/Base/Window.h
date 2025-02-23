#pragma once

#include <functional>

#include "Core/Base/Base.h"
#include "Core/Events/Event.h"
#include "Core/Renderer/Context.h"

namespace fm {

	using WindowStyle = Uint32;
	namespace Style {

		enum {

			Titlebar = BIT(0),
			Resize = BIT(1),
			Fullscreen = BIT(2)
		};

		const WindowStyle None = 0;
		const WindowStyle Default = Titlebar | Resize;
	}

	struct FM_API WindowSpecification {

		std::string Title;
		Uint32 Width;
		Uint32 Height;
		WindowStyle Style;

		WindowSpecification(const std::string& title = "FakeMinecraft Window",
			Uint32 width = 1280,
			Uint32 height = 720,
			WindowStyle style = Style::Default)
			: Title(title), Width(width), Height(height), Style(style)

		{}
	};

	class FM_API Window {

	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void OnUpdate() = 0;

		virtual std::string GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual Uint32 GetWidth() const = 0;
		virtual void SetWidth(Uint32 width) = 0;
		virtual Uint32 GetHeight() const = 0;
		virtual void SetHeight(Uint32 height) = 0;
		virtual Int32 GetX() const = 0;
		virtual void SetX(Int32 x) = 0;
		virtual Int32 GetY() const = 0;
		virtual void SetY(Int32 y) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetContext(Ref<Context> context) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetMouseCursorVisible(bool visible) = 0;
		virtual void SetMouseCursorGrabbed(bool grabbed) = 0;
		virtual void SetMouseCursorDisabled(bool disable) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Ref<Window> Create(const WindowSpecification& props = WindowSpecification());
	};
}
