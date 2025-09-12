// test_functions.c
#include <stdio.h>

// ==== 函数声明（原型声明） ====
int div(int a,int b);
int add(int a, int b),unused_function(int x, int y);           // 声明，未定义
void print_hello(void);          // 参数为空
double square(double x);         // 返回 double

// ==== 函数定义 ====
int add(int a, int b) {
    return a + b;
}

// 无参数函数
void print_hello(void) {
    printf("Hello, World!\n");
}

// 返回值为 double
double square(double x) {
    return x * x;
}

// ==== main 测试 ====
int main(void) {
    int result = add(3, 4);
    print_hello();
    double sq = square(5.0);

    printf("3 + 4 = %d\n", result);
    printf("square(5.0) = %f\n", sq);

    return 0;
}
