#include <stdio.h>
class Data {
    int n;
    Data(int x)
        : n(x)
    {   printf("Initialized n to %d\n", this->n);   }


    int square()
    {
        n *= n;
        return n;
    }
};
