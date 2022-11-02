基本的な制御構文、関数の定義と呼び出しまで対応したCのサブセットのコンパイラ。

以下はこのコンパイラが認識する言語(EBNF記法)
```
program = { declarator ident '(' [ declarator ident {',' declarator ident} ] ')' '{' {stmt} '}' }

stmt  = expr ';'
      | 'return' expr ';'
      | declarator ident ';'
      | '{' stmt* '}'
      | 'for' '(' expr? ';' expr?';' expr? ')' stmt
      | 'while' '(' expr ')' stmt
      | 'if' '(' expr ')' stmt 'else' stmt

expr  = assign

assign    = equality ('=' | '+=' | '-=' | '*=' | '/=') assign

equality  = relational ('==' | '!=') relational

relational  = add ('<=' | '>=' | '<' | '>') add

add = mul ('+' | '-') mul

mul = unary ('*' | '/') unary

unary = ('+' | '-' | '&' | { '*' }) inc

inc = ('++' | '--') primary
    | primary ('++' | '--')

primary = '(' expr ')'
        | ident [ '(' [ expr {',' expr} ] ')' ]
        | num

declarator = 'int'

ident =  alu { alu | num }

alu = al | '_'

al =  'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm'
    | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z'
    | 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M'
    | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z'

num = digit { digit }

digit = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
```
