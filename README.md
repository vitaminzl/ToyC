# ToyC语言
本项目用于学习编译原理。

将参照龙书版本的《编译原理》，

以及LLVM的编译器制作教程：https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl01.html

制作一个完整的编译器前端。 

## 编译器的结构
![](https://imagehost.vitaminz-image.top/ToyC-1.png)
<center>图1：摘自《Compilers Principles, Techniques & Tools》第二版Figure 2.3</center>

### 词法分析器(Lexer)
#### 设计Token
Token主要分为: 多字符保留字、标识符、数字以及其余单个字符。

* 多字符保留字：
  * 控制流语句：if, else, do, while, break
  * 布尔运算：true, false, &&, ||
  * 比较运算：>=, <=, ==, !=
  * 变量类型：int, float, bool, char
* 标识符：
  * 正则表达式：[\_a-zA-Z\][\_a-zA-Z0-9]*
* 数字：
  * 整型正则表达式：[0-9]+
  * 浮点型正则表达式：[0-9]+.[0-9]*
* 其余单个字字符

#### 设计类


#### 识别算法



#### 测试
输入：字符串
输出：按序输出token流，每个token占一行


词法分析器的步骤可用如下伪代码表示：
``` c
Input:  
Output: 
cache = ' '
Scan() {
    do{
        if cache != 空格、换行、制表符
            break;
    }while(read(cache));

    t = readKey(cache)     // 识别保留字
    t = readNumber(cache); // 识别实数
    t = 

    return t;
}
```

### 语法分析器


### 

## 附录

