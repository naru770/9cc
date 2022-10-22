基本的な制御構文、関数の定義と呼び出しまで対応したCのサブセットのコンパイラ。

以下はこのコンパイラが認識する言語(EBNF記法)
```
program = ( declarator ident '(' ( declarator ident (',' declarator ident)* )? ')' '{' stmt* '}' )*

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

unary = ('+' | '-' | '&' | '*') inc

inc = ('++' | '--') primary
    | primary ('++' | '--')

primary = '(' expr ')'
        | ident
        | ident '(' ( expr (',' expr)* )? ')'
        | num

ident = [a-zA-Z]*

num = [0-9]*
```
