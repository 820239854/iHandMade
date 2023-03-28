typedef struct IncludedType {
    int   a;
    char *str;
} IncludedType;

IncludedType
included_fn(int a)
{
    IncludedType result = {0};
    result.a = a;
    return result;
}
