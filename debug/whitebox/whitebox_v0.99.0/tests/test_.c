/* typedef struct Data { */
/*     int    num; */
/*     float *f; */
/* } Data; */
#define countof(a) (sizeof(a)/sizeof(*a))

int main()
{
    /* Data data = {0}; */
    float floats[] = { 3.14f, 6.28f, -1.f, 42.f, 19.8f, -23.f };
    /* float f[]  = { */
    /*     6.28f, */
    /*     3.14f, */
    /*     1.1f, */
    /* }; */
    /* char *my_str = "gosh"; */
    /* char const *strs[] = { */
    /*     "hi\nthere", */
    /*     "hello", */
    /*     "yo", */
    /* }; */
    struct {
        int n;
        float *ptr;
        char const *str;
        char   arr[20];
    } data = { .n = 4, .ptr=&floats[0] };//, .str = strs[0], .arr = "hi" };


    for (int i = 0; i < countof(floats); ++i)
    {
        data.ptr = &floats[i];
        floats[i] += 1.f;
        data.ptr = &floats[i+1 % countof(floats)];
        data.ptr = &floats[i+2 % countof(floats)];
    }

    /* for (int i = 0; i < 32; ++i) */
    /* { */
    /*     data.str = i&3 ? strs[1] : strs[2]; */
    /*     /1* if (i % 4 == 0) *1/ */
    /*     /1* {   f[1 + (i&1)] += 1.f;   } *1/ */
    /* } */

    /* for (int i = 0; i < 11; ++i) */
    /* { */
    /*     data.n = i * 2; */
    /*     if (i % 3 == 0) */
    /*     {   data.ptr = &floats[i&1];   } */

    /*     data.arr[i%sizeof(data.arr)] = 'x'; */
    /*     data.arr[sizeof(data.arr)-1] = '\0'; */
    /* } */

    return 1;
}

int
something(double (*f)(double))
{
    return f(3);
}
