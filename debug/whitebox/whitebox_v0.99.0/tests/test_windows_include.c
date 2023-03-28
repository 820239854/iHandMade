#include <windows.h>

DWORD
windows_include_fn(DWORD err_num)
{
    SetLastError(err_num);
    DWORD result = GetLastError();
    return result;
}
