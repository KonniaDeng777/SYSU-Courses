#include <iostream>
typedef char *va_list;
#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap, v) (ap = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap, type) (*(type *)((ap += _INTSIZEOF(type)) - _INTSIZEOF(type)))
#define va_end(ap) (ap = (va_list)0)

void myprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format != '\0') 
    {
        if (*format == '%') 
        {
            ++format;
            switch (*format) 
            {
                case 'd': 
                {
                    int value = va_arg(args, int);
                    std::cout<<value;
                    break;
                }
                case 's': 
                {
                    char* value = va_arg(args, char*);
                    std::cout<<value;
                    break;
                }
                default: 
                {
                    std::cout<<"Error"<<std::endl;
                    break;
                }
            }
        }
        else 
        {
            std::putchar(*format);
        }
        ++format;
    }

    va_end(args);
}

int main()
{
    myprintf("My name is %s.\n", "dengxuying");
    myprintf("My student number is %d.\n", 21307035);
    return 0;
}
