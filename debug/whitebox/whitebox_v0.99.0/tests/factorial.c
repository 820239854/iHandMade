unsigned int factorial(unsigned int n) {
    unsigned int result = 1; // NOTE: this is still correct when n=0 or n=1

    for (unsigned int i = 2; i <= n; ++i) {
        result *= i;
    }

    return result;
}
