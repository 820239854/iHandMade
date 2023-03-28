#include <stdlib.h>
typedef struct ArrayHeader {
    int    n;
    double data[1];
} ArrayHeader;

ArrayHeader *struct_hack_fn(int n, double val)
{
    ArrayHeader *array = malloc(__builtin_offsetof(ArrayHeader, data) +
                                n * sizeof(double));
    array->n = n;
    for (int i = 0; i < n; ++i)
    {   array->data[i] = val * (double)i;   }

    return array;
}

typedef struct ArrayData {
    double data[4];
} ArrayData;

ArrayData some_data()
{
    ArrayData result = { 0.1, -0.2, 0.3, -0.4 };
    return result;
}
