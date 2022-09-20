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
DECL 	-> TYPE id ';'
TYPE	-> basic DIMS
DIMS	-> '['num']'DIMS
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
	a = b + 1
L3:	t1 = 32 * 43
	t2 = a + t1
	t3 = g - 2
	t4 = b / t3
	c = t2 - t4
L4:	d = e + f
L5:	t5 = 3 * 10
	t6 = e[t5]
	c = -t6
L6:	t7 = 33 + 34
	t8 = sd3 * c
	t9 = t7 + t8
	t10 = t9 * 10
	t11 = c[t10]
	t12 = de * c
	t13 = t12 + c2
	c[t11] = t13
L7:	t14 = 2 * d
	t15 = t14 * 10
	t16 = c[t15]
	t17 = t16 + 4
	t18 = t17 * 10
	t19 = f[t18]
	t20 = f * 10
	t21 = s[t20]
	t22 = 23 - t21
	t23 = t22 * 10
	t24 = de[t23]
	t25 = df + t24
	f[t19] = t25
L8:	t26 = m * 10
	t27 = n * 10
	t28 = t26 + t27
	t29 = o * 10
	t30 = t28 + t29
	t31 = s[t30]
	t32 = 3 * 10
	t33 = x * 10
	t34 = t32 + t33
	t35 = d[t34]
	t36 = -t35
	s[t31] = t36
L9:	if False a goto L13
	t37 = 23 * 10
	t38 = f * 10
	t39 = t37 + t38
	t40 = sp[t39]
	if False t40 goto L14
	if c goto L13
	if d goto L13
	goto L14
L14:	if f goto L11
	if kk goto L13
	goto L11
L13:	t41 = true
	goto L12
L11:	t41 = false
L12:	a = t41
L10:	t42 = a + c
	t43 = b * 2
	t44 = t43 - 1
	if t42 > t44 goto L17
	if False a < b goto L15
	if c goto L17
	goto L15
L17:	t45 = true
	goto L16
L15:	t45 = false
L16:	b = t45
L2:

```


#### 控制流语句的中间代码
``` c++
PROGRAM -> BLOCK
BLOCK 	-> '{' STMTS '}'
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



输入
``` txt
{
    if ((a + c) > (b * 2 - 1) || a < b && c){
        a = b * 2 - c + (b + 2 * d);
    }
    while (a > 3){
        a = a + 1;
        do a = a + 3; while( b > 2);
        if (c - 3){
            b = c + 5;
            d = a + 3;
        }
        else {
            c = 3;
            break;
        }
    }
    while (c < b){
        a = 3;
    }
}
```



输出
```
L1:	t1 = a + c
	t2 = b * 2
	t3 = t2 - 1
	if t1 > t3 goto L5
	if False a < b goto L3
	if c goto L5
	goto L3
L5:L4:	t4 = b * 2
	t5 = t4 - c
	t6 = 2 * d
	t7 = b + t6
	a = t5 + t7
L3:	if False a > 3 goto L6
L7:	a = a + 1
L8:	a = a + 3
L10:	if b > 2 goto L8
	goto L9
L9:	if False c - 3 goto L12
L11:	b = c + 5
L13:	d = a + 3
	goto L3
L12:	c = 3
L14:	goto L6
	goto L3
L6:	if False c < b goto L2
L15:	a = 3
	goto L6
L2:

```

#### 总结



## 附录

