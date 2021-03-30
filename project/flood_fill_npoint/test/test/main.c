#include <stdio.h>

int main(void)
{
    printf("Hello World!\n");

    for (int i=0; i<100; i++)
    {
        if(i%2==0)
        {
            if(i%4==0)
            {
                printf("continue!!\n");
                continue;
            }
            printf("i=%d\n", i);
        }
        printf("-----i=%d-\n", i);


    }
    return 0;
}

