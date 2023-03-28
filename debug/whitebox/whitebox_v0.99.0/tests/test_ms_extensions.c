typedef struct Inner {
    int x, y;
} Inner;

typedef struct Outer {
    union X { Inner; Inner inner; };
    int z;
} Outer;

Outer ms_extension_fn(int x, int y, int z)
{
    Outer result = {x,y,z};
    return result;
}
