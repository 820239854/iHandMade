// loclists are when the variable is at different memory locations at different
// points in the program

typedef struct Data {
    int num;
    int do_add;
    char addends[4];
} Data;

int loclists_fn(Data data) {
    int result = 0;
    if (data.do_add)
    {   result += data.addends[data.num & 0x3];   }

    return result;
}
