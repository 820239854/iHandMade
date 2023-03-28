// non-leaf functions

int max(int a, int b)
{ return (a > b) ? a : b; }

int internal_fn_call(int a, int b, int c)
{
    int max_ab  = max(a, b);
    int max_abc = max(max_ab, c);
    return max_abc;
}
