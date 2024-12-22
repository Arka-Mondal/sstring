/*
 * Copyright (c) 2024, Arka Mondal. All rights reserved.
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 */

#ifndef _SSTRING_H_
#define _SSTRING_H_

#include <stddef.h>

#define SSTR_MAGIC 0x73737472

#define SSTR_INIT_CAP 64
#define SSTR_MAX_CAP ((size_t) -1)

typedef struct {
  size_t length;
  size_t size;
  size_t capacity;
  int magic;
  char *buf;
} _sstring_struct;

typedef char * sstring;

#define is_sstring(S) (((_sstring_struct *)((S) - sizeof(_sstring_struct)))->magic == SSTR_MAGIC)
#define sstring_slice(S, T, E) sstring_slice_with_step((S), (T), (E), 1)

sstring sstring_new(void);
sstring sstring_clone(sstring sstr);
sstring sstring_from(const char *str);
int sstring_clear(sstring sstr);
int sstring_destroy(sstring *sstr);

int sstring_sstrpush(sstring *sstr1, const sstring sstr2);
int sstring_cstrpush(sstring *sstr, const char *cstr);

int sstring_push(sstring *sstr, char ch);
sstring sstring_slice_with_step(sstring sstr, size_t start, size_t end, size_t step);

size_t sstring_length(sstring sstr);

#endif /* _SSTRING_H_ */
