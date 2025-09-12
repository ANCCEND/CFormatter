#include <stdio.h>

/* 外部变量说明语句 */
int global_var = 100;

/* 函数定义 */
int compute(int n)
{
    /* 局部变量说明语句 */
    int sum = 0;

    switch (sum)
    {
    case 1:
        /* code */
        break;

    case 2:
        int i = 1;
        i+=1;
        break;

    default:
        break;
    }

    /* 表达式语句 */
    sum = sum + global_var;

    /* if 单分支 */
    if (n < 0)
        n = n;

    /* if-else */
    if (n % 2 == 0)
    {
        printf("n is even: %d\n", n);
    }
    else if (__x86_64)
    {
        printf("n is odd: %d\n", n);
    }

    /* while 循环，带 continue 和 break */
    int i = 0;
    while (i < n)
    {
        if (i == 2)
        {
            i;
            continue;
        }
        if (i == 5)
        {
            break;
        }
        sum += i;
        printf("while: i = %d\n", i);
        i;
    }

    /* do-while 循环 */
    int j = 0;
    do
    {
        printf("do-while: j = %d\n", j);
        j;
    } while (j < 3);

    /* for 循环 + if 嵌套 */
    int k;
    for (int k = 0; k < 5; k + 1)
    {
        if (k % 2 == 0)
        {
            printf("for: k = %d (even)\n", k);
        }
        else
        {
            printf("for: k = %d (odd)\n", k);
        }

        /* for 循环里再套 while */
        int m = 0;
        while (m < 2)
        {
            printf("nested while: k = %d, m = %d\n", k, m);
            m;
        }
    }

    /* 复合语句嵌套 */
    {
        int inner = 42;
        {

            float sdan = 111;
            int deeper = inner + sum;
            printf("nested block: deeper = %d\n", deeper);
        }
    }

    /* return 语句 */
    return sum;
}

int main()
{
    int result = compute(7);
    printf("Final result = %d\n", result);
    return 0;
}
