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
        root.Offset({ 0, 0, 0 });
        m_target.Root(root);

        // Create some test visuals
        if (m_target.Root())
        {
            auto visuals = m_target.Root().as<winrt::WUIC::ContainerVisual>().Children();

            auto tl{ m_compositor.CreateSpriteVisual() };
            tl.Brush(m_compositor.CreateColorBrush({ 128, 255, 0, 0 }));
            tl.Size({ 50, 50 });
            tl.AnchorPoint({ 0, 0 });
            tl.RelativeOffsetAdjustment({ 0, 0, 0 });
            visuals.InsertAtTop(tl);

            auto tr{ m_compositor.CreateSpriteVisual() };
            tr.Brush(m_compositor.CreateColorBrush({ 128, 0, 255, 0 }));
            tr.Size({ 50, 50 });
            tr.AnchorPoint({ 1, 0 });
            tr.RelativeOffsetAdjustment({ 1, 0, 0 });
            visuals.InsertAtTop(tr);

            auto bl{ m_compositor.CreateSpriteVisual() };
            bl.Brush(m_compositor.CreateColorBrush({ 128, 0, 0, 255 }));
            bl.Size({ 50, 50 });
            bl.AnchorPoint({ 0, 1 });
            bl.RelativeOffsetAdjustment({ 0, 1, 0 });
            visuals.InsertAtTop(bl);

            auto br{ m_compositor.CreateSpriteVisual() };
            br.Brush(m_compositor.CreateColorBrush({ 128, 128, 0, 128 }));
            br.Size({ 50, 50 });
            br.AnchorPoint({ 1, 1 });
            br.RelativeOffsetAdjustment({ 1, 1, 0 });
            visuals.InsertAtTop(br);

            auto c{ m_compositor.CreateSpriteVisual() };
            c.Brush(m_compositor.CreateColorBrush({ 128, 255, 255, 255 }));
            c.Size({ 50, 50 });
            c.AnchorPoint({ 0.5, 0.5 });
            c.RelativeOffsetAdjustment({ 0.5, 0.5, 0 });
            visuals.InsertAtTop(c);
        }
        // /TEST
    }
#pragma endregion Public

#pragma region Private
#pragma endregion Private
}