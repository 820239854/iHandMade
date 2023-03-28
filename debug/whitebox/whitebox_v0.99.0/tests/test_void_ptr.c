int *int_cpy(int *dst, int const *src, int n)
{
    for (int i = 0; i < n; ++i)
    {
        dst[i] = src[i];
    }
    return dst;
}







void *void_cpy(void *dst, void const *src, int n)
{
    char *d = dst, *s = src;
    for (int i = 0; i < n; ++i)
    {
        d[i] = s[i];
    }
    return d;
}

typedef struct MyStruct MyStruct;

void *simple_ptr(MyStruct *in)
{
    return in;
}
