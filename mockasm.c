/*
 *
 * mockasm.c
 *
 * Created at:  Fri 09 Aug 2013 20:33:49 CEST 20:33:49
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License: the MIT license
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>

#include "mockasm.h"

/* The Registers */
union regs_t regs;

/* the very first op from which the execution starts */
struct op_t *op_root;

/* the first label */
struct labels_t *labels;

/* bison crap */
extern int yyparse(void);
extern FILE *yyin;
extern FILE *yyout;

int main(int argc, char *argv[])
{
  /* name of the input file */
  char fname[255];
  /* pointer to that file */
  FILE *fptr;

  if (argc < 2){
    printf("usage: mockasm <input file>\n");
    exit(1);
  }

  strcpy(fname, argv[1]);

  /* try to open the file */
  if ((fptr = fopen(fname, "r")) != NULL){
    yyin = fptr;
  } else {
    perror(fname);
    exit(1);
  }

  yyparse();

  /* execute the ops */
  for (struct op_t *op = op_root; op != NULL; op = op->next)
    if (op->func)
      EXECOP(op);

  /* clean-up the ops */
  struct op_t *op, *next;
  for (op = op_root; op != NULL; op = next){
    next = op->next;
    free(op);
  }

  return 0;
}

struct op_t *newNOP(void)
{
  struct op_t *op = mmalloc(sizeof(struct op_t));

  op->type = OP_NOP;
  op->func = NULL;

  return op;
}

struct unop_t *newPRINT(word_t arg)
{
  struct unop_t *op = mmalloc(sizeof(struct unop_t));

  op->type = OP_NOP;
  op->func = execPRINT;
  op->arg = arg;

  return (struct op_t *)op;
}

void execPRINT(struct op_t *op)
{
  printf("%d\n", ((struct unop_t *)op)->arg);
}

void new_label(const char *name, struct op_t *op)
{
  struct labels_t *label = mmalloc(sizeof(struct labels_t));

  label->name = strdup(name);
  label->op = op;
  labels = label;
  label->next = labels;
}

struct op_t *get_label(const char *name)
{
  for (struct labels_t *l = labels; l != NULL; l = l->next){
    printf("get_label: cmp %s, %s\n", l->name,name);
    if (!strcmp(l->name, name)){
      return l->op;
    }
  }

  return NULL;
}

void *mmalloc(size_t size)
{
  void *m = malloc(size);

  if (!m){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  return m;
}

