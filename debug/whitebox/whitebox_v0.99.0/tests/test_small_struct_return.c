// small struct returns
typedef struct SmallStruct {
    char          a;
    unsigned char b;
} SmallStruct;

SmallStruct small_struct_return(char a, unsigned char b)
{
    SmallStruct result = {0};
    result.a = a;
    result.b = b;
    return result;
}
