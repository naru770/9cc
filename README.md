基本的な制御構文、関数の定義と呼び出しまで対応したCのサブセットのコンパイラ。

以下はこのコンパイラが認識する言語(EBNF記法)
```
program = { func }

func = type ident '(' type ident { ',' type idnet } ')' '{' [ stmt ] '}'

stmt  = expr ';'
      | 'return' expr ';'
      | declarator ident ';'
      | '{' [ { stmt } ] '}'
      | 'for' '(' [ expr ] ';' [ expr ] ';' [ expr ] ')' stmt
      | 'while' '(' expr ')' stmt
      | 'if' '(' expr ')' stmt 'else' stmt

expr  = assign

assign = equality { ('=' | '+=' | '-=' | '*=' | '/=') assign }

equality  = relational { ('==' | '!=') relational }

relational  = add { ('<=' | '>=' | '<' | '>') add }

add = mul { ('+' | '-') mul }

mul = unary { ('*' | '/') unary }

unary = [ ('+' | '-' | '&' | { '*' }) ] inc

inc = [ ('++' | '--') ] primary
    | primary [ ('++' | '--') ]

primary = '(' expr ')'
        | ident [ '(' [ expr { ',' expr } ] ')' ]
        | num

type = 'int'

ident =  alu { alu | num }

alu = al | '_'

al =  [a-zA-Z]

num = digit { digit }

digit = [0-9]
```