#ifndef _MURMURHASH_H_
#define _MURMURHASH_H_

#include "Value.h"

#define ROTL32(x,y) rotl32(x,y)

uint32_t getblock32(const uint32_t *p, int i);

uint32_t fmix32(uint32_t h);

uint32_t rotl32(uint32_t x, int8_t r);

void MurmurHash3_uint32_uint32(const uint32_t key, uint32_t seed, uint32_t *out);

uint32_t MurmurHash3_x86_32 (const void * key, int len, uint32_t seed);

#endif// _MURMURHASH_H
