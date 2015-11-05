#ifndef _BLOOMFILTER_H_
#define _BLOOMFILTER_H_
#include "Value.h"

void storeBloomFilter(struct ipaddress (*ip)[TEN_BIT], unsigned char* key, uint32_t hash, uint32_t *size, uint32_t prefix);

void checkBloomFilter(struct BloomFilter (*bf)[TEN_BIT], uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix, int *matchvector);

void deleteBloomFilter(struct ipaddress (*ip)[TEN_BIT], struct BloomFilter (*bf)[TEN_BIT],unsigned char* key, uint32_t hash, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix);

void checkFalsePositive(struct ipaddress (*ip)[TEN_BIT], struct BloomFilter (*bf)[TEN_BIT], unsigned char* key, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix, int *matchvector);

#endif// _BLOOMFILTER_H
