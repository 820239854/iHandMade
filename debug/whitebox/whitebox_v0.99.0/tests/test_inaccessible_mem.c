#include <stdint.h>
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
void *mem_reserve(size_t size)           { return VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE); }
void *mem_commit(void *ptr, size_t size) { return VirtualAlloc(ptr,  size, MEM_COMMIT,  PAGE_READWRITE); }
void  mem_protect(void *ptr, size_t size, int access) {   unsigned long d; VirtualProtect(ptr, size, (access ? PAGE_READWRITE : PAGE_NOACCESS), &d);   }

#else
#include <sys/mman.h>
void *mem_reserve(size_t size)           { return mmap(0, size, PROT_NONE, (MAP_PRIVATE | MAP_ANON | MAP_NORESERVE), -1, 0); }
void *mem_commit(void *ptr, size_t size) { return (mprotect(ptr, size, (PROT_READ | PROT_WRITE)) == 0 ? ptr : 0); }
void  mem_protect(void *ptr, size_t size, int access) {   mprotect(ptr, size, (access ? (PROT_READ | PROT_WRITE) : PROT_NONE));   }
#endif

void inaccessible_mem_fn()
{
    int *ptr = 0;
    ptr = mem_reserve(4096);
    mem_commit(ptr, 4096);
    *ptr = 0;
    ++*ptr;

    mem_protect(ptr, 4096, 0);
    /* ++*ptr; */
    mem_protect(ptr, 4096, 1);
    ++*ptr;
}
