/* #define _NO_CRT_STDIO_INLINE */
#include <stdio.h>

void
stdouts_cde(void)
{
    printf("out c\n");
    fprintf(stderr, "err d\n");
    fprintf(stdout, "out e\n");
}


void
print_stds(void)
{
    puts("out a");
    fputs("err b\n", stderr);
    printf("out flush!\n");
    fflush(stdout);

    stdouts_cde();

    fprintf(stderr, "err f"); // no newline
    fprintf(stdout, "out g\n");
}


void
print_to_file(void)
{
    FILE *file = fopen("test.c", "w");
    fprintf(stderr, "file: %p\n", file);
    if (file)
    {
        fprintf(file, "writing to file");
    }
}

int p()
{
    return fprintf(stdout,"JIT stdout: %p\n", stdout);
}

FILE *get_stdout()
{
    return stdout;
}
