// Loop breakpoint testing

void loops()
{
    int i = 6;
    while (--i > 0);

    int k = 0;
    for (int j = 0; j < 14; ++j)
    {
        k += j*j;
    }

    {   int a_scope = 6;   }
    {   int b_scope = 8;   }

    --k;

    int l = 0;
loop:
    if (++l < 5)
    { goto loop; }
}
