#ifndef _MURMURHASH_H_
#define _MURMURHASH_H_

#include "Value.h"

#define ROTL32(x,y) rotl32(x,y)

uint32_t getblock32(const uint32_t *p, int i);

uint32_t fmix32(uint32_t h);

uint32_t rotl32(uint32_t x, int8_t r);

void MurmurHash3_uint32_uint32(const uint32_t key, uint32_t seed, uint32_t *out);

#endif// _MURMURHASH_H
