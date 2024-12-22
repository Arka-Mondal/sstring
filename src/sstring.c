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
  n |= n >> 32;

  return ++n;
}

static void * sstring_resize_buffer(_sstring_struct *sstr_strct, size_t newcap)
{
  if (newcap <= sstr_strct->capacity)
    newcap = SSTR_MAX_CAP;

  return realloc(sstr_strct, sizeof(_sstring_struct) + newcap * sizeof(char));
}

sstring sstring_new(void)
{
  _sstring_struct *sstr;

  sstr = malloc(sizeof(_sstring_struct) + SSTR_INIT_CAP * sizeof(char));
  if (sstr == NULL)
    return NULL;

  sstr->length = 0;
  sstr->size = 1;
  sstr->capacity = SSTR_INIT_CAP;
  sstr->magic = SSTR_MAGIC;
  sstr->buf = (char *) sstr + sizeof(_sstring_struct);
  sstr->buf[0] = '\0';

  return sstr->buf;
}

sstring sstring_from(const char *str)
{
  size_t size, capacity;
  _sstring_struct *sstr;

  size = strlen(str) + 1;
  capacity = next_power_of_2(size);

  sstr = malloc(sizeof(_sstring_struct) + capacity * sizeof(char));
  if (sstr == NULL)
    return NULL;

  sstr->length = size - 1;
  sstr->size = size;
  sstr->capacity = capacity;
  sstr->magic = SSTR_MAGIC;
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
  _sstring_struct *sstr_strct1, *sstr_strct2;

  if (sstr1 == NULL || *sstr1 == NULL || sstr2 == NULL)
    return -1;

  sstr_strct1 = (_sstring_struct *) (*sstr1 - sizeof(_sstring_struct));
  sstr_strct2 = (_sstring_struct *) (sstr2 - sizeof(_sstring_struct));

  if (sstr_strct1->magic != SSTR_MAGIC || sstr_strct2->magic != SSTR_MAGIC)
    return -1;

  if (sstr_strct1->capacity < (sstr_strct1->size + sstr_strct2->length))
  {
    newcap = next_power_of_2(sstr_strct1->size + sstr_strct2->length);
    // sstr_strct1 = realloc(sstr_strct1, sizeof(_sstring_struct) + newcap * sizeof(char));
    sstr_strct1 = sstring_resize_buffer(sstr_strct1, newcap);
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
  _sstring_struct *sstr_strct;

  if ((sstr == NULL) || (*sstr == NULL) || (cstr == NULL) ||
    ((sstr_strct = (_sstring_struct *) (*sstr - sizeof(_sstring_struct)), sstr_strct->magic) != SSTR_MAGIC))
    return -1;

  cstr_len = strlen(cstr);

  if (sstr_strct->capacity < sstr_strct->size + cstr_len)
  {
    newcap = next_power_of_2(sstr_strct->size + cstr_len);
    // sstr_strct = realloc(sstr_strct, sizeof(_sstring_struct) + newcap * sizeof(char));
    sstr_strct = sstring_resize_buffer(sstr_strct, newcap);
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

int sstring_push(sstring *sstr, char ch)
{
  size_t newcap;
  _sstring_struct *sstr_strct;

  if ((sstr == NULL) || (*sstr == NULL) ||
    ((sstr_strct = (_sstring_struct *) (*sstr - sizeof(_sstring_struct)), sstr_strct->magic) != SSTR_MAGIC))
    return -1;

  if (sstr_strct->capacity < sstr_strct->size + 1)
  {
    newcap = next_power_of_2(sstr_strct->size + 1);
    // sstr_strct = realloc(sstr_strct, sizeof(_sstring_struct) + newcap * sizeof(char));
    sstr_strct = sstring_resize_buffer(sstr_strct, newcap);
    if (sstr_strct == NULL)
      return -1;

    sstr_strct->capacity = newcap;
    sstr_strct->buf = (char *) sstr_strct + sizeof(_sstring_struct);
    *sstr = sstr_strct->buf;
  }

  sstr_strct->buf[sstr_strct->length] = ch;
  sstr_strct->buf[sstr_strct->size] = '\0';

  sstr_strct->length++;
  sstr_strct->size++;

  return 0;
}

size_t sstring_length(sstring sstr)
{
  _sstring_struct *sstr_strct;

  if ((sstr == NULL) ||
    ((sstr_strct = (_sstring_struct *) (sstr - sizeof(_sstring_struct)), sstr_strct->magic) != SSTR_MAGIC))
    return 0;

  return sstr_strct->length;
}

sstring sstring_slice_with_step(sstring sstr, size_t start, size_t end, size_t step)
{
  size_t slice_limit;
  _sstring_struct *sstr_strct;
  sstring nstr;

  if ((sstr == NULL)
    || ((sstr_strct = (_sstring_struct *) (sstr - sizeof(_sstring_struct)), sstr_strct->magic) != SSTR_MAGIC))
    return NULL;

  nstr = sstring_new();
  if (nstr == NULL)
    return NULL;

  slice_limit = (end < sstr_strct->length) ? end : sstr_strct->length;

  for (size_t k = start; k < slice_limit; k += step)
    if (sstring_push(&nstr, sstr_strct->buf[k]) == -1)
      break;

  return nstr;
}

sstring sstring_clone(sstring sstr)
{
  if (sstr == NULL)
    return NULL;

  return sstring_slice(sstr, 0, sstring_length(sstr));
}

int sstring_clear(sstring sstr)
{
  _sstring_struct *sstr_strct;

  if ((sstr == NULL)
    || ((sstr_strct = (_sstring_struct *) (sstr - sizeof(_sstring_struct)), sstr_strct->magic) != SSTR_MAGIC))
    return -1;

  sstr_strct->length = 0;
  sstr_strct->size = 0;


  return 0;
}
