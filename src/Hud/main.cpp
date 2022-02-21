#include "pch.h"
#include <iostream>

int Run(HINSTANCE /*hInstance*/)
{
    winrt::init_apartment();

    std::wcout << L"Hello world!" << std::endl;

    return 0;
}

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