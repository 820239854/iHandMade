float
vars_arithmetic(float a, float t, float b)
{
    float intermediate = b - a;
    float result = a + t * intermediate;
    return result;
}

float indirect(float t)
{
    return vars_arithmetic(0, t, 10);
}

int basic(int a)
{
    ++a;
    int x = 0;
    ++a;
    ++a;
    return a;
}

int result_only(int a)
{
    return a;
}

int x2(int a)
{
    return a * 2;
}
