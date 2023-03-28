// `do while` with a non-constant condition and a variable within scope
// appears to force a .debug_rnglists section

void rnglists_fn()
{
    int a=0, b=0;

    do
    {
        int v1 = 1;
        int v2 = 2;
        ++v1;
    }
    while (a != b);
}
