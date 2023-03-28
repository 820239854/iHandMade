typedef enum Enum {
    Enum_unknown = -1,
    Enum_a       = 0,
    Enum_b       = 1,
    Enum_c       = 2,
    Enum_c2      = 2,
    Enum_d       = 3,
    Enum_e       = 4,
    Enum_f       = 5,
    Enum_count,
} Enum;

Enum test_enum(int a)
{
    Enum result = a < Enum_count ? a : Enum_unknown;
    return result;
}

typedef enum Flags {
    Flags_unknown = 0,
    Flags_a       = 1 << 0,
    Flags_b       = 1 << 1,
    Flags_c       = 1 << 2,
    Flags_c2      = 1 << 2,
    Flags_d       = 1 << 3,
    Flags_e       = 1 << 4,
    Flags_f       = 1 << 5,
    Flags_Count
} Flags;

Flags test_enum_flags(int a, int b, int c, int d, int e, int f)
{
    Flags result = ((!!a) << 0 |
                    (!!b) << 1 |
                    (!!c) << 2 |
                    (!!d) << 3 |
                    (!!e) << 4 |
                    (!!f) << 5);
    return result;
}

