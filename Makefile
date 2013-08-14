CC = gcc
CFLAGS = -W -Wall -Wextra -g -std=gnu99
YACC = bison
LEX = flex

OBJS = mockasm.c lex.yy.c parse.tab.c

all: lexer parser mockasm

mockasm: $(OBJS)
	$(CC) $(CFLAGS) -o mockasm $(OBJS)

parser: parse.y
	$(YACC) -d parse.y

lexer: lex.l
	$(LEX) lex.l

clean:
	rm -f *.o
	rm -f *.tab.*
	rm -f *.yy*

distclean: clean
	rm -f mockasm

