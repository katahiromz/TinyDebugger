#include <windows.h>

int main(void)
{
    OutputDebugStringA("OK1\n");
    OutputDebugStringW(L"OK2\n");
    MessageBoxA(NULL, "hello, world", NULL, MB_ICONINFORMATION | MB_YESNOCANCEL);
    MessageBoxW(NULL, L"2nd", NULL, MB_ICONINFORMATION | MB_YESNOCANCEL);
    return 0;
}
