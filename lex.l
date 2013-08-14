%option noinput
%option nounput

%{

  #include "stdio.h"
  #include "mockasm.h"
  #include "parse.tab.h"

%}

%%

ax            { return AX; }
bx            { return BX; }
cx            { return CX; }
dx            { return DX; }
ah            { return AH; }
al            { return AL; }
bh            { return BH; }
bl            { return BL; }
ch            { return CH; }
cl            { return CL; }
dh            { return DH; }
dl            { return DL; }

nop           { return NOP; }
print         { return PRINT; }
push          { return PUSH; }
pop           { return POP; }
mov           { return MOV; }
add           { return ADD; }
jmp           { return JMP; }

[+-]?[0-9]+   { yylval.w = atoi(yytext); return INT; }
[a-zA-Z][a-zA-Z0-9_]+ { yylval.s = strdup(yytext); return NAME; }

"\n"          { return '\n'; }
","           { return ','; }
";"           { return ';'; }
":"           { return ':'; }

[ \t]         { /* skip whitespace */ }

%%

int yywrap()
{
  return 1;
}

