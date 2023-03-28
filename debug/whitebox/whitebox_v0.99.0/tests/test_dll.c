#include <windows.h>
#include <stdio.h>

int main(void) {
    HMODULE module = LoadLibraryA("C:\\Windows\\System32\\ucrtbase.dll");
    if (! module) printf("Not loaded!\n");
    void *proc = (void*)GetProcAddress(module, "__acrt_iob_func");
    printf("%p\n", proc);
    return 0;
}
