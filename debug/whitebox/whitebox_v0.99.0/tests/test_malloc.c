// malloc
#include <stdlib.h>

double *malloc_fn(double val)
{
    double *d = (double *)malloc(sizeof(double));
    *d = val + 3;
    return d;
}

int *malloc_int_fn()
{
    int *i = (int *)malloc(sizeof(int));
    *i = 3;
    return i;
}

double *malloc_array_fn(double val, int n)
{
    double *d = (double *)malloc(n * sizeof(double));

    for (int i = 0; i < n; ++i)
    {   d[i] = val * (double)(i+1);   }

    // TODO: d[.] shows as no data unless something is referenced here
    //       this is despite the deref of the return showing a value
    //       presumably there's some optimization going on that we want to disable
    /* double x = d[0]; */

    return d;
}
