%error-verbose

%{

  #include "stdio.h"
  #include "stdlib.h"
  #include "mockasm.h"

  void yyerror(const char *s);
  int yylex();

  static inline void push(word_t value);
  static inline word_t pop(void);

  extern union regs_t regs;
  extern struct op_t *op_root;
  static struct op_t *prev_op;

  #define STACK_SIZE 32

  word_t stack[STACK_SIZE] = { 0 };
  word_t stack_ptr = 0;

%}

%union {
  word_t w;
  word_t *wp;
  char *s;
  struct op_t *op;
}

%token INT NAME
%token NOP PRINT PUSH POP MOV ADD JMP
%token AX BX CX DX AH AL BH BL CH CL DH DL

%type <w> INT
%type <s> NAME
/*%type <w> AX BX CX DX AH AL BH BL CH CL DH DL*/
%type <wp> reg
%type <op> line stmt

%%

input
    : /* null */
    | input line {
        op_root = op_root ? op_root : $2;
        if (prev_op)
          if (!prev_op->next)
            prev_op->next = $2;
        prev_op = $2;
      }
    ;

line
    : terminator { $$ = newNOP(); }
    | stmt terminator { $$ = $1; }
    ;

stmt
    : NOP { $$ = newNOP(); }
    | PUSH rval { $$ = newNOP(); }
    | POP reg { $$ = newNOP(); }
    | PRINT rval {
        $$ = newPRINT(pop());
      }
    | MOV reg ',' rval {
        word_t v = pop();
        *($2) = v;
        $$ = newNOP();
      }
    | ADD reg ',' rval {
        word_t v = pop();
        *($2) += v;
        $$ = newNOP();
      }
    | NAME '=' rval {
        $$ = newNOP();
      }
    | NAME ':' '\n' stmt { $$ = $4; new_label($1, $4); }
    | JMP NAME { $$ = newNOP(); $$->next = get_label($2); printf("JMP next: %p\n", $$->next); }
    ;

reg
    : AX { $$ = &regs.word.ax; }
    | BX { $$ = &regs.word.bx; }
    | CX { $$ = &regs.word.cx; }
    | DX { $$ = &regs.word.dx; }
    | AH { $$ = &regs.byte.ah; }
    | AL { $$ = &regs.byte.al; }
    | BH { $$ = &regs.byte.bh; }
    | BL { $$ = &regs.byte.bl; }
    | CH { $$ = &regs.byte.ch; }
    | CL { $$ = &regs.byte.cl; }
    | DH { $$ = &regs.byte.dh; }
    | DL { $$ = &regs.byte.dl; }
    ;

rval
    : INT  { push($1);    }
    | NAME { push(4);     }
    | reg  { push(*($1)); }
    ;

terminator
    : '\n'
    | ';'
    ;

%%

void yyerror(const char *s)
{
  fprintf(stderr, "HOLY MOLY: %s\n", s);

  exit(1);
}

static inline void push(word_t value)
{
  if (stack_ptr > STACK_SIZE){
    fprintf(stderr, "stack overflow!\n");
    exit(1);
  }

  stack[stack_ptr++] = value;
}

static inline word_t pop(void)
{
  if (stack_ptr <= 0){
    fprintf(stderr, "stack underflow!\n");
    exit(1);
  }

  return stack[--stack_ptr];
}

