class Data {
public:
    int a;
    int b;
private:
    int c;
};


int class_fn(Data data)
{
    data.a += data.b;
    return data.a;
}

/* int use_class_fn() */
/* { */
/*     Data d {2,3}; */
/*     return class_fn(d); */
/* } */
