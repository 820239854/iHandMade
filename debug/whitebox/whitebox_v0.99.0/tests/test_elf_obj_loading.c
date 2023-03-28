// object loading

int obj_fn(int a);

int elf_obj_loading(int a)
{
    int added = a + 4;
    int result = obj_fn(added);
    return result;
}
