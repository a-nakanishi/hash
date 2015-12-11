#ifndef _BLOOMFILTER_H_
#define _BLOOMFILTER_H_

#include "Value.h"

void storeStage1BloomFilter(struct Name *list, struct BloomFilter (*bf)[TEN_BIT]);

void checkStage1BloomFilter(struct BloomFilter (*bf)[TEN_BIT], int pref_len, uint32_t num1, uint32_t num2, uint32_t num3, int *MatchVector);

void makeMergedBloomFilter(struct MergedBloomFilter *mbf, unsigned char *key, uint32_t hash1, uint32_t hash2, uint32_t hash3, int port_num);

void checkMergedBloomFilter(struct MergedBloomFilter *mbf, uint32_t num1, uint32_t num2, uint32_t num3, int *flug, int *interface_num);

#endif// _BLOOMFILTER_H
