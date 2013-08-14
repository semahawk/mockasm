/*
 *
 * mockasm.h
 *
 * Created at:  Fri 09 Aug 2013 21:01:54 CEST 21:01:54
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

#ifndef MOCKASM_H
#define MOCKASM_H

#include <endian.h>

typedef unsigned char byte_t;
typedef unsigned short word_t;

union regs_t {
  struct {
    word_t ax, bx, cx, dx, ip;
  } word;
  struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    byte_t al, ah, bl, bh, cl, ch, dl, dh;
#else
    byte_t ah, al, bh, bl, ch, cl, dh, dl;
#endif
  } byte;
};

enum op_type_t {
  OP_NOP,
  OP_PRINT
};

#define EXECOP(op) (op->func(op))

#define OPHEAD \
  enum op_type_t type; \
  void (*func)(struct op_t *); \
  struct op_t *next; \

struct op_t {
  OPHEAD
};

struct unop_t {
  OPHEAD
  word_t arg;
};

struct binop_t {
  OPHEAD
  word_t first;
  word_t second;
};

struct op_t *newNOP(void);
struct unop_t *newPRINT(word_t arg);
void execPRINT(struct op_t *);

void *mmalloc(size_t);

/* singly linked list of any defined labels */
struct labels_t {
  const char *name;
  struct op_t *op;
  struct labels_t *next;
};

void new_label(const char *name, struct op_t *op);
struct op_t *get_label(const char *name);

#endif /* MOCKASM_H */

