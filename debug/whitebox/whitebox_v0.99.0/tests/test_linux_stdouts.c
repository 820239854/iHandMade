#pragma comment(lib, "c")
#pragma comment(lib, "dl")
#include <stdio.h>
#include <dlfcn.h>

int main()
{
    void *self = dlopen(NULL, RTLD_NOW);
    void *lib  = dlopen("dl", RTLD_NOW);
    printf("stdout:      %p\n"
           "self_stdout: %p\n"
           "lib__stdout: %p\n"
           ,stdout
           ,dlsym(self, "stdout")
           ,dlsym(lib,  "stdout")
    );
}
