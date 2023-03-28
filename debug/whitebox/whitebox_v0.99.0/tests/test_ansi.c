#include <stdio.h>

# define A_Prefix "\x1b["
// Effects - optional
# define A_Normal "0;"
# define A_Bold "1;"
# define A_Underline "4;"
// foreground/background and dim (lo)/bright (hi) - pick 1
# define A_FG_lo "3"
# define A_FG_hi "9"
# define A_BG_lo "4"
# define A_BG_hi "10"
// colour - pick 1
# define A_Black  "0"
# define A_Red    "1"
# define A_Green  "2"
# define A_Yellow "3"
# define A_Blue   "4"
# define A_Purple "5"
# define A_Cyan   "6"
# define A_White  "7"
# define A_Esc(s) A_Prefix s "m"
# define ANSI_Reset  A_Prefix "0m"

# define ANSI(col, s) col s ANSI_Reset
# define ANSI_Esc(col, s) A_Esc(col) s ANSI_Reset

# define ANSI_Lo_Black(s)  ANSI(A_Esc(A_Normal A_FG_lo A_Black),  s)
# define ANSI_Lo_Red(s)    ANSI(A_Esc(A_Normal A_FG_lo A_Red),    s)
# define ANSI_Lo_Green(s)  ANSI(A_Esc(A_Normal A_FG_lo A_Green),  s)
# define ANSI_Lo_Yellow(s) ANSI(A_Esc(A_Normal A_FG_lo A_Yellow), s)
# define ANSI_Lo_Blue(s)   ANSI(A_Esc(A_Normal A_FG_lo A_Blue),   s)
# define ANSI_Lo_Purple(s) ANSI(A_Esc(A_Normal A_FG_lo A_Purple), s)
# define ANSI_Lo_Cyan(s)   ANSI(A_Esc(A_Normal A_FG_lo A_Cyan),   s)
# define ANSI_Lo_White(s)  ANSI(A_Esc(A_Normal A_FG_lo A_White),  s)

# define ANSI_Hi_Black(s)  ANSI(A_Esc(A_Normal A_FG_hi A_Black),  s)
# define ANSI_Hi_Red(s)    ANSI(A_Esc(A_Normal A_FG_hi A_Red),    s)
# define ANSI_Hi_Green(s)  ANSI(A_Esc(A_Normal A_FG_hi A_Green),  s)
# define ANSI_Hi_Yellow(s) ANSI(A_Esc(A_Normal A_FG_hi A_Yellow), s)
# define ANSI_Hi_Blue(s)   ANSI(A_Esc(A_Normal A_FG_hi A_Blue),   s)
# define ANSI_Hi_Purple(s) ANSI(A_Esc(A_Normal A_FG_hi A_Purple), s)
# define ANSI_Hi_Cyan(s)   ANSI(A_Esc(A_Normal A_FG_hi A_Cyan),   s)
# define ANSI_Hi_White(s)  ANSI(A_Esc(A_Normal A_FG_hi A_White),  s)

void test_ansi_sgr() {
    puts("\n\nnormal");
    // CSI /////////////////////////////////////////////////////////////////////
    // SGR
    // colors
    puts("\e[31;106mred on cyan - graphic design is my hobby\e[m");
    puts("\e[34mblue");
    puts("\e[32mgreen bleed");
    fputs("continue on stderr\n", stderr);

    puts("overwrite\r\e[31mnew");
    puts("next line\n");

    // palette
    for (int i = 0; i < 8; ++i)
    {
        printf("\e[m[%d] "
            "\e[3%dmforeground lo\e[m | \e[9%dmforeground hi\e[m | "
            "\e[97;4%dm background lo \e[m | \e[30;10%dm background hi \e[m\n",
            i, i, i, i, i);
    }

    puts("\e[38:2:0:255:255many \e[48:2:255:255:128;38:2mRGB\e[49;38:2:255:150:150m color\e[m\n");

    // lines
    puts("\e[4msingle underline");
    puts("\e[4:2mdouble underline");
    puts("\e[4:3mcurly\e[58:2:255::m red \e[59munderline");
    puts("\e[4:4;59mdotted underline");
    puts("\e[4:5mdashed underline");
    puts("\e[21mdouble underline");
    puts("\e[m\e[9mstrikethrough");
    puts("\e[29mstrike off");
    puts("\e[m\e[53moverline\e[m\n");

    // effects
    puts("\e[1mbold");
    puts("a reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeaaaaaalllllllllyyyyy long bit of text that's just bound to overflow...\e[m");
    puts("\e[5mslow blink");
    puts("\e[6mfast blink");
    puts("\e[25;8mhidden \e[28mtext");
    puts("\e[7mreversed");
    puts("\e[27mreverse off");
}

void
test_ansi_cursor()
{
    // CURSOR
    // grid
    puts("\e[0m\n\n");
    puts("[0] ......................................................");
    puts("[1] ......................................................");
    puts("[2] ......................................................");
    puts("[3] ......................................................");
    puts("[4] ......................................................");
    puts("[5] ......................................................");
    puts("[6] ......................................................");
    puts("[7] ......................................................");
    puts("[8] ......................................................");
    puts("[9] ......................................................");
    puts("[a] ......................................................");
    puts("[b] ......................................................");
    puts("[c] ......................................................");
    puts("[d] ......................................................");
    puts("[e] ......................................................");
    puts("[f] ......................................................");


    // movements
    fputs("\e[6Cright",   stdout);
    fputs("\e[13Aup",     stdout);
    fputs("\e[16CR",      stdout);
    fputs("\e[1Bdown",    stdout);
    fputs("\e[12Dleft\n", stdout);

    // Independent control functions

    // Control strings
}


int main()
{
    test_ansi_sgr();
    test_ansi_cursor();
    return 0;
}
