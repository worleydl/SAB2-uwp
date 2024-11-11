#include <windows.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.ViewManagement.Core.h>
#include <winrt/Windows.Graphics.Display.Core.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.System.h>

#include "../NewSonicThing/src/engineTester/main.hpp"

using namespace winrt;
using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Graphics::Display::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

typedef void (*KeyCallback)(void*, int c, int scancode, int action, int mods);
static KeyCallback p_goKeyCB = NULL;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
	IFrameworkView CreateView()
	{
		return *this;
	}

	void Initialize(CoreApplicationView const& v)
	{
		v.Activated({ this, &App::OnActivate });
	}

	void OnActivate(const winrt::Windows::ApplicationModel::Core::CoreApplicationView&,
		const winrt::Windows::ApplicationModel::Activation::IActivatedEventArgs& args)
	{
		if (args.Kind() == Windows::ApplicationModel::Activation::ActivationKind::Protocol)
		{
			// TODO Launchpass setup
		}

	retry:
		CoreWindow window = CoreWindow::GetForCurrentThread();
		if (window == NULL)
			goto retry;

		window.Activate();
	}

	void Load(hstring const&)
	{
	}

	void Uninitialize()
	{
	}

	void Run()
	{
		auto navigation = winrt::Windows::UI::Core::SystemNavigationManager::GetForCurrentView();
		navigation.BackRequested(
			[](const winrt::Windows::Foundation::IInspectable&,
				const winrt::Windows::UI::Core::BackRequestedEventArgs& args) { args.Handled(true); });

		SAB2::ExternalEntry(0, 0);
	}

	void SetWindow(CoreWindow const& window)
	{
		// No-op
	}
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPCSTR argstr, int)
{
	winrt::init_apartment();
	CoreApplication::Run(make<App>());
	winrt::uninit_apartment();
}