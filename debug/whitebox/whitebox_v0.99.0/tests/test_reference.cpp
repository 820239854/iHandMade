void reference(int &x)
{
    x = x * x;
}

struct Data {
    int y;
    int &x;
};

void ref_struct(Data &ref, float &f)
{
    ref.y += 6;
    ref.x += 3;
    f *= 2;
}
