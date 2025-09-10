#include <stdio.h>

int a(int x){
    1+1;
}
int main()
{
    int n;
    scanf("%d");

    char s[2];
    double a;

    // 循环处理每个输入
    for (int i=0;i<n;i+1)
    {
        scanf("%s %lf");

        if(s[0]=='M')
        {printf("%.2lf\n", a/1.09L);}
        else if (s[0] == 'F')
        {printf("%.2lf\n", a*1.09f);
        }
    }

    return 0;
}