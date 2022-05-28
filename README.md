# ToyC语言
本项目用于学习编译原理。

将参照龙书版本的《编译原理》，

以及LLVM的编译器制作教程：https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl01.html

制作一个完整的编译器前端。

## 编译器的结构
![](https://imagehost.vitaminz-image.top/ToyC-1.png)
<center>图1：摘自《Compilers Principles, Techniques & Tools》第二版Figure 2.3</center>

### UML

![](https://imagehost.vitaminz-image.top/UML.png)

<center>图2：UML</center>



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


#### 识别算法

```

```

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

### 语法分析器(Parser)

```c++
PROGRAM -> BLOCK
BLOCK 	-> '{' DECLS STMTS '}'
DECLS 	-> DECLS DECL
    	-> eps
DECL 	-> TYPE id';'
TYPE	-> basic DIMS
DIMS	-> '['DIMS']'
	-> eps
STMTS	-> STMTS STMT
    	-> eps

STMT	-> ASSIGN';'
	-> if ( BOOL ) STMT
        -> if ( BOOL ) STMT else STMT
        -> while ( BOOL ) STMT
        -> do STMT while ( BOOL )
        -> break';'
        -> BLOCK
ASSIGN  -> id OFFSET = BOOL
OFFSET  -> [ BOOL ] OFFSET
        -> eps
            
BOOL	-> BOOL "||" JOIN
        -> JOIN
JOIN	-> JOIN "&&" EQAULITY
        -> EQUALITY
EQUALITY-> EQUALITY "==" CMP
        -> EQUALITY "!=" CMP
        -> CMP
CMP	-> EXPR < EXPR
        -> EXPR <= EXPR
        -> EXPR >= EXPR
        -> EXPR > EXPR
        -> EXPR
EXPR	-> EXPR + TERM
        -> EXPR - TERM
        -> TERM
TERM	-> TERM * UNARY
        -> TERM / UNARY
        -> UNARY
UNARY	-> '!' UNARY
        -> '-' UNARY
        -> FACTOR
FACTOR	-> ( BOOL )
        -> id OFFSET
        -> number
        -> real
        -> true
        -> false
```



### 符号表(Symbol Table)

<img src="https://imagehost.vitaminz-image.top/ToyC-2.png" style="zoom:50%;" />

<center>图2：符号表示意图</center>

```c++
PROGRAM -> {top = null;} BLOCK
BLOCK -> '{' 
		{ saved = top;				// 保留现场，saved
		  top = new Scope(top); } 	// 碰到块建立符号表，top指向当前块符号表
		DECLS DECL 
		{ top = saved; }			// 恢复现场，
		'}'
DECLS -> DECLS DECL
      -> eps
DECL  -> TYPE id';' { s = new Symbol(id);			// 
					  s.type = TYPE.lexeme;
					  top.put(id.lexeme, s); }
TYPE  -> basic {DIMS.type = basic; } 
		 DIMS {TYPE.lexeme = DIMS.type; }
DIMS  -> '['num']' DIMS { Array.sz = num * Array.sz;
    					  DIMS.type = Array; }
      -> eps { Array.sz = 1; 
               Array.type = Dims.type; }
STMTS -> STMTS STMT 
      -> eps
STMT  -> BLOCK
STMT  -> .... > ... id { s = top.get(id.lexeme); } ....
```





单元测试：

```c++
PROGRAM -> BLOCK
BLOCK 	-> '{' DECLS STMTS '}'
DECLS 	-> DECLS DECL
    	-> eps
DECL 	-> TYPE id';'
TYPE	-> basic DIMS
DIMS	-> '['DIMS']'
		-> eps
STMTS	-> STMTS STMT
    	-> eps
STMT	-> BLOCK
    	-> FACTOR ';'
FACTOR	-> id
```



### 中间代码(Intermediate Code)

#### 表达式的计算
<img src="https://imagehost.vitaminz-image.top/ToyC-3.png" style="zoom: 25%;" />

<img src="https://imagehost.vitaminz-image.top/ToyC-5.png" 
style="zoom: 25%;" />

``` c++
PROGRAM -> STMTS
STMTS	-> STMTS STMT
    	-> eps
STMT	-> ASSIGN';'
ASSIGN  -> id OFFSET = BOOL
OFFSET  -> [ BOOL ] OFFSET
        -> eps

BOOL	-> BOOL "||" JOIN
        -> JOIN
JOIN	-> JOIN "&&" EQAULITY
        -> EQUALITY
EQUALITY-> EQUALITY "==" CMP
        -> EQUALITY "!=" CMP
        -> CMP
CMP	-> EXPR < EXPR
        -> EXPR <= EXPR
        -> EXPR >= EXPR
        -> EXPR > EXPR
EXPR	-> EXPR + TERM
        -> EXPR - TERM
        -> TERM
TERM	-> TERM * UNARY
        -> TERM / UNARY
        -> UNARY
UNARY	-> '!' UNARY
        -> '-' UNARY
        -> FACTOR
FACTOR	-> ( BOOL )
        -> OFFSET
        -> number
        -> real
        -> true
        -> false
        -> id OFFSET
```

输入

``` txt
a = b + 1;
c = a + 32 * 43 - b / ( g - 2);
d = e + f;
c = -e[3];
c[33+34+sd3*c] = de*c + c2;
f[c[2*d]+4] = df + de[23-s[f]];
s[m][n][o] = -d[3][x];
a = !a || s && (c || d) || !f && kk ;
b = (a + c) > (b * 2 - 1) || a < b && c;
```

输出

``` txt 

```



#### 跳转语句的中间代码
``` c++
STMTS	-> STMTS STMT
    	-> eps

STMT	-> ASSIGN';'
	-> if ( BOOL ) STMT
        -> if ( BOOL ) STMT else STMT
        -> while ( BOOL ) STMT
        -> do STMT while ( BOOL )
        -> break';'
        -> BLOCK
ASSIGN  -> id OFFSET = BOOL
OFFSET  -> [ BOOL ]
        -> eps
            
BOOL	-> BOOL "||" JOIN
        -> JOIN
JOIN	-> JOIN "&&" EQAULITY
        -> EQUALITY
EQUALITY-> EQUALITY "==" CMP
        -> EQUALITY "!=" CMP
        -> CMP
CMP	-> EXPR < EXPR
        -> EXPR <= EXPR
        -> EXPR >= EXPR
        -> EXPR > EXPR

```

#### 总结









## 附录

