#include <stdio.h>
#include <uchar.h>
#include <vector>

struct Stuff {
    std::vector<int> ints;
    unsigned best_int_i;
};


int
external_template_fn(Stuff *stuff)
{
    int result = -1;
    if (stuff->best_int_i < stuff->ints.size())
    {
        result = stuff->ints[stuff->best_int_i];
    }
    return result;
}

int main()
{
    Stuff stuff = {};
    stuff.ints.push_back(8);
    printf("%d\n", external_template_fn(&stuff));
    return 0;
}
