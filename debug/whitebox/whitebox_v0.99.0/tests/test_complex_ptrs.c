// unusual pointers

typedef struct DataType {
    int i;
    float f;
} DataType;

typedef struct PtrType PtrType;
struct PtrType {
    DataType *data;
    PtrType  *prev_ptr;
};

typedef struct PtrTypeContainer {
    PtrType ptr_data[3];
    PtrType *ptrs[3];
} PtrTypeContainer;
    

int complex_ptrs()
{
    DataType data[] = {
        { 1, 3.14f },
        { 2, 4.14f },
        { 3, 5.14f },
    };

    typedef PtrType PtrTypeArray[3];

    PtrTypeArray ptrs = {{ &data[0], 0 }};
    ptrs[1].data     = &data[1];
    ptrs[1].prev_ptr = &ptrs[0];
    ptrs[2].data     = &data[2];
    ptrs[2].prev_ptr = &ptrs[1];

    PtrTypeContainer container = { { ptrs[0], ptrs[1], ptrs[2], }, { ptrs, ptrs+1, ptrs+2 } };

    PtrTypeArray *ptrs_ptr = &ptrs;
    int *data_1 = &ptrs_ptr[0][1].data->i;
    *data_1 = 4;

    int last = 999;
    return 0;
}
