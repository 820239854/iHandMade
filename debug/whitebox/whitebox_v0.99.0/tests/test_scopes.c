// Scope and initialization testing

int global_scope = 42;
int global_scope_uninit;
static int global_scope_static = 3;

int scopes()
{
    int multiple_scopes = -1;
    int main_scope      =  0;
    int uninitialized_1;
    int uninitialized_2;

    static int in_fn_static = 9;

    for (int i = 0; i < 2; ++i)
    {
        {
            int inner_scope     = 2;
            main_scope          = 1;
            int multiple_scopes = 3;
            ++multiple_scopes;
            uninitialized_1     = 4;
            int end_of_scope    = 5;
        }

        { int single_line_scope = 8; }

        uninitialized_2 = 6;
        int post_scope = 7; // TODO: should this be dumped with the others before the scope or after?

        {
            ++main_scope;
            int pre_return_scope = 8;
        }
    }

    ++main_scope;
    return 0;
}
