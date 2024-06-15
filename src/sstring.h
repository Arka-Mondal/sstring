/*
 * Copyright (c) 2024, Arka Mondal. All rights reserved.
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 */

#ifndef _SSTRING_H_
#define _SSTRING_H_

#include <stddef.h>

typedef struct {
  size_t length;
  size_t size;
  size_t capacity;
  char *buf;
} _sstring_struct;

typedef char * sstring;

sstring sstring_from(const char *str);
int sstring_destroy(sstring *sstr);

int sstring_sstrpush(sstring *sstr1, const sstring sstr2);
int sstring_cstrpush(sstring *sstr, const char *cstr);

#endif /* _SSTRING_H_ */
