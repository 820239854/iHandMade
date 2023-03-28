// local include

#include "test_local_included.c"

IncludedType
local_include(int a, char *str)
{
    IncludedType result = included_fn(a + 2);
    result.str = str;
    return result;
}
