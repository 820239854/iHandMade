// simple function prototypes

double
simple_protos_lerp(double a, double t, double b)
{   return a + t * (b - a);   }

typedef void *Addr;

typedef struct Type {
    int    x, y;
    double z;
    void  *addr;
    char  *str;
} MyType;

MyType
simple_protos_struct(int x, int y, double z, Addr addr, char *str)
{
    ++x;
    MyType result = { x, y, z, addr, str };
    ++result.x;
    return result;
}

MyType
simple_protos_struct2(int x)
{
    MyType result = { x };
    return result;
}

MyType
struct_return(MyType a)
{
    MyType copy = a;
    return a;
}
