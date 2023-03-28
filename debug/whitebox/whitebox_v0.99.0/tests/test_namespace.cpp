#if 1
namespace other_ns
{
    typedef int OtherInt;
    struct Data { OtherInt val; };
}

namespace outer_ns
{
    typedef int OuterInt;
    namespace inner_ns
    {
        other_ns::Data fn(OuterInt a)
        {
            using namespace other_ns;
            Data result = { a * a };
            return result;
        }
    }
    namespace another_ns
    {
        int fn(int a, int b)
        {
            return a + b;
        }
        struct UnusedData { int x; };
    }
}

#else
typedef int OtherInt;
struct Data { OtherInt val; };

Data fn(OtherInt a)
{
    Data result = { a * a };
    return result;
}
#endif
