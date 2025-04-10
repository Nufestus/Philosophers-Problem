#include "philosophers.h"

int main()
{
    int i;

    i = 0;
    while (i < 10)
    {
        if (i == 5)
            i = 10;
        i++;
        printf("%d\n", i);
    }
}