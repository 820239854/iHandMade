// complex nested types

typedef float f32;
struct FnStruct {
    enum { Fn_int, Fn_float, } kind;
    union {
        int i;
        f32 f;
    };
};
typedef int Fn(struct FnStruct info, int data);
typedef Fn *MyFn;

typedef char *Str;
typedef Str MyStr;

typedef int UnusedType;

typedef struct RecursiveTypeTag RecursiveType;
struct RecursiveTypeTag {
    int data;
    RecursiveType *next;
};
typedef struct RecursiveTypeTag *RecursiveTypePtr;

typedef enum {
    ANON_a,
    ANON_b,
} AnonEnum;

typedef enum TaggedEnumTag {
    TAG_a,
    TAG_b,
} TaggedEnum;

enum TagOnlyEnum {
    Only_a,
    Only_b,
};


typedef struct {
    AnonEnum data;
    TaggedEnum data2;
    enum TagOnlyEnum data3;
} Anon;

typedef Anon const  ConstAnon;
typedef Anon const *ConstAnonPtr;

typedef RecursiveType (*const RecursiveFn1)(int data, RecursiveType *prev);
typedef RecursiveType (**const * RecursiveFn2[3])(int data, RecursiveType *prev);

typedef unsigned char uint8_t;
typedef uint8_t       Byte;

typedef struct Type {
    int     x, y;
    double  z;
    void   *addr;
    Str     str;

    RecursiveTypePtr re;

    Fn     *td_fn;
    MyFn    my_fn;
    void    (*fn_ptr)();
    void    (*fn_ptr2)(RecursiveFn1 fn);
    int    *ptr;
    int     arr[3];

    Anon         anon;
    Anon         anons[2];
    ConstAnon    const_anon;
    ConstAnonPtr const_anon_ptr;

    RecursiveType    recursive;
    RecursiveFn1     recursive_fn1;
    RecursiveFn2     recursive_fn2;
} MyType;

struct Container {
    MyType my_type;
    int other_data[2];

    Byte byte;

    struct NamedStruct { MyType my_type; int data; } named_struct;

    struct { int data; struct Type my_types[1][2][3][4]; } inline_struct;

    struct { int anon_data; MyType const *my_type_ptr; };
};


struct Container
complex_types(int x, int y, double z, void *addr, MyStr str)
{
    struct Container container  = { x, y, z, addr, str },
                     container2 = container;
    return container2;
}

/* int main() */
/* { */
/*     struct Container container = fn(0, 0, 0, 0, 0); */
/*     return container.my_type.x; */
/* } */
