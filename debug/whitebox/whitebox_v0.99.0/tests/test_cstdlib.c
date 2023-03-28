#include <stdlib.h>

int int_cmp(void const *a, void const *b)
{   return *(int const *)a - *(int const *)b;   }

int test_qsort()
{
    int array[] = { 1, 9, 3, 12, -1, 15 };
    qsort(array, sizeof(array)/sizeof(*array), sizeof(*array), int_cmp);
    return array[0];
}
