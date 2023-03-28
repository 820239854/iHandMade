int even_pointed_to_fn(int a)
{   return 2 * a;   }

int odd_pointed_to_fn(int a)
{   return 47 * a;   }

int fn_ptr(int a, int fn_i)
{
    typedef int (*Fn)(int);
    Fn  fn[2]  = { even_pointed_to_fn, odd_pointed_to_fn };
    int is_odd = fn_i & 1;
    int result = fn[is_odd](a);
    return result;
}
