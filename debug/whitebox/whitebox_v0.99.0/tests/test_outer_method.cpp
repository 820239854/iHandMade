class Data {
public:
    int a;
    int b;

    int method(int d);

private:
    int c;
};

int Data::method(int d)
{
    c = a + b + d;
    int result = c;
    return result;
}
