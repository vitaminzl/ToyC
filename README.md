# ToyC语言
本项目用于学习编译原理。

将参照龙书版本的《编译原理》，

以及LLVM的编译器制作教程：https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl01.html

制作一个完整的编译器前端。 

## 编译器的结构
![](https://imagehost.vitaminz-image.top/ToyC-1.png)
<center>图1：摘自《Compilers Principles, Techniques & Tools》第二版Figure 2.3</center>

### 词法分析器(Lexer)
词法分析器的步骤可用如下伪代码表示：
``` c
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


