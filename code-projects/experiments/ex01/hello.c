#include<stdio.h>

void input() {
    char str[100];
    
    printf("请输入内容：");
    scanf("%s", str); 
    
    printf("您输入的是：%s\n", str);
}

int main()
{
    printf("Hello,world!\n");
    input();
    return 0;
}