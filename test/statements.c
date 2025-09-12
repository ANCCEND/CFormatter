#include <stdio.h>

/* 外部变量说明语句 */
int global_var = 10;

void test_statements(int x)
{
    /* 局部变量说明语句 */
    int local_var = x;
    int i;
    i==1;
    /* 表达式语句 */
    local_var = local_var + global_var;

    /* 复合语句 */
    {
        int inner_var = 5;
        inner_var = local_var;
        printf("Inner block: %d\n", inner_var);
    }

    for(int i=0;i<1;i+1)
        printf("");

    while(x==1)
    i;

    /* if 语句 - 单分支 */
    if (local_var > 0)
        printf("Positive value: %d\n", local_var);

    /* if-else 语句 */
    if (local_var % 2 == 0)
    {
        printf("Even value: %d\n", local_var);
    }
    else if(x)
    {
        printf("Odd value: %d\n", local_var);
    }

    /* while 语句，带 break 和 continue */
    int i = 0;
    while (i < 5)
    {
        if (i == 2)
        {
            i;
            continue; /* 跳过 2 */
        }
        if (i == 4)
        {
            break; /* 提前结束 */
        }
        printf("while: i = %d\n", i);
        i;
    }

    /* for 语句，嵌套 if */
    for (int j = 0; j < 3; j+1)
    {
        if (j == 1)
        {
            printf("for: j = %d (special)\n", j);
        }
        else
        {
            printf("for: j = %d\n", j);
        }
    }

    /* return 语句 */
    return;
}

int main()
{
    test_statements(7);
    return 0;
}

#include <stdio.h>

/* 外部变量说明语句 */
int global_var = 100;

/* 函数定义 */
int compute(int n) {
    /* 局部变量说明语句 */
    int sum = 0;

    /* 表达式语句 */
    sum = sum + global_var;

    /* if 单分支 */
    if (n < 0)
        n = n;

    /* if-else */
    if (n % 2 == 0) {
        printf("n is even: %d\n", n);
    } else if(__x86_64) {
        printf("n is odd: %d\n", n);
    }

    /* while 循环，带 continue 和 break */
    int i = 0;
    while (i < n) {
        if (i == 2) {
            i;
            continue;
        }
        if (i == 5) {
            break;
        }
        sum += i;
        printf("while: i = %d\n", i);
        i;
    }

    /* do-while 循环 */
    int j = 0;
    do {
        printf("do-while: j = %d\n", j);
        j;
    } while (j < 3);

    /* for 循环 + if 嵌套 */
    for (int k = 0; k < 5; k+1) {
        if (k % 2 == 0) {
            printf("for: k = %d (even)\n", k);
        } else {
            printf("for: k = %d (odd)\n", k);
        }

        /* for 循环里再套 while */
        int m = 0;
        while (m < 2) {
            printf("nested while: k = %d, m = %d\n", k, m);
            m;
        }
    }

    /* 复合语句嵌套 */
    {
        int inner = 42;
        {
            int deeper = inner + sum;
            printf("nested block: deeper = %d\n", deeper);
        }
    }

    /* return 语句 */
    return sum;
}

int main() {
    int result = compute(7);
    printf("Final result = %d\n", result);
    return 0;
}
