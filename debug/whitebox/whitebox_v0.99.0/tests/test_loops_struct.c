// Loop breakpoint testing

void loops()
{
    int i = 6;
    while (--i > 0);

    int a = 0, b = 1, *a_ptr = &a;

    struct {int k; float f; } st = { 0 };
    for (int j = 0; j < 14; ++j)
    {
        st.k += j*j;
        if (j & 1)
        {
            float f = 6.14;
            st.f += 2.f;
        }
    }

    a_ptr = &b;
    *a_ptr = 3;

    --st.k;

    int l = 0;
loop:
    if (++l < 5)
    { goto loop; }
}
