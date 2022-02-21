#include "pch.h"

#include "CompositionHost.h"

#include <iostream>
#include <memory>

namespace
{
    constexpr std::wstring_view c_windowTitle{ L"Hud" };
    constexpr std::wstring_view c_windowClass{ L"HudWindowClass" };
    HINSTANCE g_hInstance;
    std::unique_ptr<Hud::CompositionHost> c_compositionHost{ nullptr };
}

// Forward declarations
int Run(HINSTANCE hInstance);
ATOM RegisterWindowClass(HINSTANCE hInstance);
void InitializeWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Console entrypoint
int main(
    int /*argc*/,
    wchar_t* /*argv*/[]
)
{
    return Run(GetModuleHandleW(nullptr));
}

// Windows entrypoint
int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE /*hPrevInstance*/,
    LPWSTR /*lpCmdLine*/,
    int /*nCmdShow*/
)
{
    return Run(hInstance);
}

// Initializes window and runs the message loop
int Run(HINSTANCE hInstance)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    RegisterWindowClass(hInstance);
    InitializeWindow(hInstance);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

ATOM RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASSW windowClass
    {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WndProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = hInstance,
        .hIcon = nullptr,
        .hCursor = LoadCursorW(nullptr, IDC_ARROW),
        .hbrBackground = nullptr,
        .lpszMenuName = nullptr,
        .lpszClassName = c_windowClass.data(),
    };

    return RegisterClass(&windowClass);
}

void InitializeWindow(HINSTANCE hInstance)
{
    g_hInstance = hInstance;

    HWND hWnd = CreateWindowW(
        c_windowClass.data(),
        c_windowTitle.data(),
        WS_POPUP,
        0,
        0,
        400,
        800,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd)
    {
        throw std::runtime_error("Could not create window!");
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    c_compositionHost = std::make_unique<Hud::CompositionHost>(hWnd);
}

LRESULT CALLBACK WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}
