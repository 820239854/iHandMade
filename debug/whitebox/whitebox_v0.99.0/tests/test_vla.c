#include <stdlib.h>

typedef struct {
    int len;
    char buf[];
} VLA;

void test_vla(VLA *arr)
{
    for (int idx = 0; idx < arr->len; idx++)
    {
        arr->buf[idx] = (char)idx;
    }
}

int main()
{
    return 0;
}
