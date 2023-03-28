typedef struct MyStruct {
    int         is[4];
    float      *fs;
    long        fs_n;
    char const *str;
} MyStruct;

void loop_structs_fn()
{
    float fs[][4] = {
        { -1.f, 0.f, 6.28f, 42.f },
        { -1.f, 1.f, 3.14f,  0.f },
        { -1.f, 2.f, 6.28f,  0.f },
        { -1.f, 3.f, 6.28f,  0.f }};
    MyStruct my          = {0};
    char     local_str[] = "local string";
    my.fs_n              = sizeof(fs)/sizeof(*fs);

    for (int i = 0; i < my.fs_n; ++i)
    {
        my.is[i] = 100 + i;
        my.fs    = &fs[i][1];
        my.str   = local_str;
    }
}
