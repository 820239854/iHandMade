// windows include
#define WIN32_LEAN_AND_MEAN
/* #include <windows.h> */
void SetLastError(unsigned int);
unsigned int GetLastError(void);

int windows_last_error()
{
    int result = 4;
    SetLastError(123);
    result = GetLastError();
    return result;
}
