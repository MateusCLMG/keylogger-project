#include <windows.h>
#include <iostream>

HHOOK hHook;

LPARAM FDS;

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LONG_PTR lParam) {
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*) lParam;
        std::cout << "Key Pressed: " << kbdStruct->vkCode << std::endl;
        std::cout << "Timestamp: " << kbdStruct->time << std::endl;
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void SetHook() {
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, GetModuleHandle(NULL), 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int main() {
    SetHook();
    return 0;
}