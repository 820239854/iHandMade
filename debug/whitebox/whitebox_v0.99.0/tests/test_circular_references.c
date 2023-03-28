// Testing circular references

typedef struct Link Link;
struct Link {
    int   data;
    Link *next;
};

void circular_references()
{
    Link link1 = { 1 },
         link2 = { 2, &link1 };
    link1.next = &link2;
    ++link2.data;
}



typedef struct LinkInt LinkInt;
typedef struct LinkFloat LinkFloat;
struct LinkInt {
    int   data;
    LinkFloat *next;
};
struct LinkFloat {
    LinkInt *next;
    float    data;
};

void longer_circular_references()
{
    LinkInt   link1 = { 1 };
    LinkFloat link2 = { &link1, 6.28 };
    link1.next = &link2;
    ++link1.data;
}
