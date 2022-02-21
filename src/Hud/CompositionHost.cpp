#include "pch.h"

#include "CompositionHost.h"

#include <DispatcherQueue.h>
#include <windows.ui.composition.interop.h>

namespace winrt
{
    namespace WS = Windows::System;
    namespace WUIC = Windows::UI::Composition;
    namespace WUICD = Windows::UI::Composition::Desktop;
}

namespace
{
#pragma region Free functions
    inline winrt::WS::DispatcherQueueController CreateDispatcherQueue()
    {
        DispatcherQueueOptions options
        {
            .dwSize = sizeof(DispatcherQueueOptions),
            .threadType = DQTYPE_THREAD_CURRENT,
            .apartmentType = DQTAT_COM_ASTA,
        };

        winrt::WS::DispatcherQueueController controller{ nullptr };
        winrt::check_hresult(CreateDispatcherQueueController(
            options,
            reinterpret_cast<ABI::Windows::System::IDispatcherQueueController**>(
                winrt::put_abi(controller))
        ));

        return controller;
    }

    inline winrt::WUICD::DesktopWindowTarget CreateDesktopWindowTarget(
        HWND window,
        winrt::WUIC::Compositor compositor
    )
    {
        namespace abi = ABI::Windows::UI::Composition::Desktop;

        auto interop = compositor
            .as<abi::ICompositorDesktopInterop>();
        winrt::WUICD::DesktopWindowTarget target{ nullptr };
        winrt::check_hresult(interop->CreateDesktopWindowTarget(
            window,
            false,
            reinterpret_cast<abi::IDesktopWindowTarget**>(winrt::put_abi(target))
        ));
        return target;
    }
#pragma endregion Free functions
}

namespace Hud
{
#pragma region Public
    CompositionHost::CompositionHost(HWND hWnd) :
        m_dispatcherQueueController{ CreateDispatcherQueue() },
        m_compositor{ winrt::WUIC::Compositor{} },
        m_target{ CreateDesktopWindowTarget(hWnd, m_compositor) }
    {
        auto root{ m_compositor.CreateContainerVisual() };
        root.RelativeSizeAdjustment({ 1.0f, 1.0f });
        root.Offset({ 124, 12, 0 });
        m_target.Root(root);

        // TEST
        if (m_target.Root())
        {
            auto visuals = m_target.Root().as<winrt::WUIC::ContainerVisual>().Children();

            auto element = m_compositor.CreateSpriteVisual();
            element.Brush(m_compositor.CreateColorBrush({ 128, 255, 0, 0 }));
            element.Size({ 50, 50 });
            element.Offset({ 16, 16, 0 });
            visuals.InsertAtTop(element);
        }
        // /TEST
    }
#pragma endregion Public
    
#pragma region Private
#pragma endregion Private
}