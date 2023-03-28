int
assert_fn(int ok, int val)
{
    if (! ok)
    {
        __builtin_debugtrap();
    }

    return val;
}
