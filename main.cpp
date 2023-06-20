#include <iostream>
#include <string>
#include <Windows.h>

HHOOK hookHandle = NULL;  // Global variable to store the hook handle

// Hook procedure
LRESULT CALLBACK HookProcedure(int code, WPARAM wParam, LPARAM lParam)
{
    // Check if the hook code indicates a message being processed
    if (code >= 0 && wParam && lParam)
    {
        // Cast the parameters to the appropriate types
        MSG* message = reinterpret_cast<MSG*>(lParam);

        // TODO: Handle the hooked messages here
        // You can perform custom processing based on the window messages

        // Call the next hook procedure in the chain
        return CallNextHookEx(hookHandle, code, wParam, lParam);
    }

    // Call the next hook procedure in the chain
    return CallNextHookEx(hookHandle, code, wParam, lParam);
}

// Function to hook a DLL into a specific window
bool HookDLLToWindow(HWND windowHandle, const char* dllPath)
{
    // Load the DLL
    HMODULE dllHandle = LoadLibrary(dllPath);
    if (dllHandle == NULL)
    {
        std::cout << "Failed to load the DLL." << std::endl;
        return false;
    }

    // Get the address of the hook procedure
    HOOKPROC hookProc = reinterpret_cast<HOOKPROC>(GetProcAddress(dllHandle, "HookProcedure"));
    if (hookProc == NULL)
    {
        std::cout << "Failed to get the address of the hook procedure." << std::endl;
        FreeLibrary(dllHandle);
        return false;
    }

    // Install the hook procedure
    hookHandle = SetWindowsHookEx(WH_CALLWNDPROC, hookProc, dllHandle, 0);
    if (hookHandle == NULL)
    {
        std::cout << "Failed to install the hook procedure." << std::endl;
        FreeLibrary(dllHandle);
        return false;
    }

    std::cout << "DLL hooked successfully to the window." << std::endl;

    // Wait for messages until the window is closed or 'unhook' is entered
    MSG message;
    while (GetMessage(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);

        // Check for user input to unhook the DLL
        if (message.message == WM_KEYDOWN)
        {
            if (message.wParam == 'U' && GetAsyncKeyState(VK_SHIFT) & 0x8000)
            {
                std::cout << "Unhook command received. Unhooking the DLL." << std::endl;
                UnhookWindowsHookEx(hookHandle);
                FreeLibrary(dllHandle);
                break;
            }
        }
    }

    return true;
}

int main()
{
    // Obtain the handle of the specific window you want to hook
    HWND windowHandle = FindWindow(NULL, "Window Title");  // Replace "Window Title" with the actual title of the window

    if (windowHandle == NULL)
    {
        std::cout << "Window not found." << std::endl;
        return 1;
    }

    const char* dllPath = "path/to/your/dll.dll";  // Replace with the path to your DLL

    // Hook the DLL to the specific window
    if (!HookDLLToWindow(windowHandle, dllPath))
    {
        std::cout << "Failed to hook DLL to the window." << std::endl;
        return 1;
    }

    std::cout << "DLL hooking complete." << std::endl;

    return 0;
}
