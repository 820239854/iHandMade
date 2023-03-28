#include <math.h>
#include <stdio.h>

float fn(float f)
{
    printf("arg: %f", f);
    return cosf(f);
}
