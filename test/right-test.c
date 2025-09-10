// 测试文件：C语言子集语法分析
// 行注释测试

/* 块注释测试
   这里是多行注释
*/

#include <stdio.h> // 文件包含
#define PI 3.14    // 宏定义

// 外部变量说明
int globalVar = 10;
const int MAX = 100;
int arr[5]; // 数组变量

// 函数声明
int add(int a, int b);
void printMessage(void);

// 主函数
int main(void)
{
    // 局部变量
    int i = 0;
    int sum = 0;
    int nums[3] = {1, 2, 3};

    // 表达式语句 + 算术运算符
    sum = add(nums[0], nums[1]) + nums[2] * 2;

    // if 语句（单分支）
    if (sum > MAX)
    {
        sum = MAX;
    }

    // if-else 语句
    if (sum == 0)
    {
        printMessage();
    }
    else if (sum == 1)
    {
        sum = sum + 1;
    }
    else
        return 1;

    // while 语句
    while (i < 5)
    {
        sum = sum + i;
        i = i + 1;
    }

    // for 语句
    for (int i = 0; i < 5; i = i + 1)
    {
        if (i == 3)
        {
            continue; // 跳过当前循环
        }
        if (i == 4)
        {
            break; // 提前退出循环
        }
        sum = sum + i;
    }

    // return 语句
    return sum;
}

// 函数定义
int add(int a, int b)
{
    return a + b;
}

void printMessage(void)
{
    printf("Hello, C subset test!\n");
}
