/*
 * Copyright (c) 2024, Arka Mondal. All rights reserved.
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "sstring.h"

static size_t next_power_of_2(size_t n)
{
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;

  return ++n;
}

sstring sstring_from(const char *str)
{
  size_t size, capacity;
  _sstring_struct * sstr;

  size = strlen(str) + 1;
  capacity = next_power_of_2(size);

  sstr = malloc(sizeof(_sstring_struct) + capacity * sizeof(char));
  if (sstr == NULL)
    return NULL;

  sstr->length = size - 1;
  sstr->size = size;
  sstr->capacity = capacity;
  sstr->buf = (char *) sstr + sizeof(_sstring_struct);
  strcpy(sstr->buf, str);

  return sstr->buf;
}

int sstring_destroy(sstring *sstr)
{
  *sstr -= sizeof(_sstring_struct);
  free(*sstr);
  *sstr = NULL;

  return 0;
}

int sstring_sstrpush(sstring *sstr1, const sstring sstr2)
{
  size_t newcap;
  _sstring_struct * sstr_strct1, * sstr_strct2;

  sstr_strct1 = (_sstring_struct *) (*sstr1 - sizeof(_sstring_struct));
  sstr_strct2 = (_sstring_struct *) (sstr2 - sizeof(_sstring_struct));

  if (sstr_strct1->capacity < (sstr_strct1->size + sstr_strct2->length))
  {
    newcap = next_power_of_2(sstr_strct1->size + sstr_strct2->length);
    sstr_strct1 = realloc(sstr_strct1, sizeof(_sstring_struct) + newcap * sizeof(char));
    if (sstr_strct1 == NULL)
      return -1;

    sstr_strct1->capacity = newcap;
    sstr_strct1->buf = (char *) sstr_strct1 + sizeof(_sstring_struct);
    *sstr1 = sstr_strct1->buf;
  }

  strcpy(*sstr1 + sstr_strct1->length, sstr2);

  sstr_strct1->length += sstr_strct2->length;
  sstr_strct1->size += sstr_strct2->length;

  return 0;
}


int sstring_cstrpush(sstring *sstr, const char *cstr)
{
  size_t cstr_len, newcap;
  _sstring_struct * sstr_strct;

  sstr_strct = (_sstring_struct *) (*sstr - sizeof(_sstring_struct));
  cstr_len = strlen(cstr);

  if (sstr_strct->capacity < sstr_strct->size + cstr_len)
  {
    newcap = next_power_of_2(sstr_strct->size + cstr_len);
    sstr_strct = realloc(sstr_strct, sizeof(_sstring_struct) + newcap * sizeof(char));
    if (sstr_strct == NULL)
      return -1;

    sstr_strct->capacity = newcap;
    sstr_strct->buf = (char *) sstr_strct + sizeof(_sstring_struct);
    *sstr = sstr_strct->buf;
  }

  strcpy(*sstr + sstr_strct->length, cstr);

  sstr_strct->length += cstr_len;
  sstr_strct->size += cstr_len;

  return 0;
}
