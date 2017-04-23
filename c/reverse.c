void reverse_word(char* str)
{
    char *p;
    int start, end;

    reverse_str(str, 0, strlen(str)-1);

    p = str;
    start = 0;
    end = 0;

    while('\0' != *str)
    {
        if(*str == ' ')
        {
            reverse_str(p, start, end-1);
            start = end + 1;
            p = str;
        }
        str++;
        end++;
    }

    return;
}

void reverse_str(char* str, int start, int end)
{
    char tmp;

    if ( start < 0 || start >= end)
    {
        return;
    }

    while(start < end)
    {
        tmp = str[start];
        str[start] = str[end];
        str[end] = tmp;
        end--;
        start++;
    }

    return;
}
