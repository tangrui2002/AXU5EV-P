#include <stdio.h>        // 标准输入输出
#include "math_utils.h"   // 包含自定义头文件

int main() {
    int x = 5, y = 3;
    int result = add(x, y);
    printf("%d + %d = %d\n", x, y, result);
    return 0;
}
