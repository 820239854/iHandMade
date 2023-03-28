
int string_param(char const *str)
{
    int first_char = str[0];
    return first_char;
}

char const *Global_Strs[] = {
    "ahoy",
    "hello",
    "how are you?",
    "sup",
};

char const * string_return(int str_i)
{
    char const *result = Global_Strs[str_i];
    result =Global_Strs[1+str_i];
    return result;
}
