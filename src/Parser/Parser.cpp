#include <iostream>
// PROGRAM -> STMTS
// STMTS	-> STMTS STMT
//     	-> eps
// STMT	-> ASSIGN';'
// ASSIGN  -> id OFFSET = BOOL
// BOOL	-> BOOL "||" JOIN
//         -> JOIN
// JOIN	-> JOIN "&&" EQAULITY
//         -> EQUALITY
// EQUALITY-> EQUALITY "==" CMP
//         -> EQUALITY "!=" CMP
//         -> CMP
// CMP	-> EXPR < EXPR
//         -> EXPR <= EXPR
//         -> EXPR >= EXPR
//         -> EXPR > EXPR
// EXPR	-> EXPR + TERM
//         -> EXPR - TERM
//         -> TERM
// TERM	-> TERM * UNARY
//         -> TERM / UNARY
//         -> UNARY
// UNARY	-> '!' UNARY
//         -> '-' UNARY
//         -> FACTOR
// FACTOR	-> ( BOOL )
//         -> LOC
//         -> number
//         -> real

