#include <windows.h>

int *alloc(unsigned size)
{
    int *mem = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    return mem;
}
