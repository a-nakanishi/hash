#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "Value.h"

void makeFIB(struct Name *list, unsigned char *key, unsigned char *interface ,uint32_t *port);

void makeInterface(struct MergedBloomFilter (*mbf)[FORWARDING_PORT_NUM], unsigned char *key, unsigned char *interface, uint32_t *port);

void checkInterface(struct MergedBloomFilter (*mbf)[FORWARDING_PORT_NUM], unsigned char *key, int *MatchVector, uint32_t *default_router, uint32_t *interface_num);

void LongestPrefixMatching(struct Name *list, struct BloomFilter (*bf)[TEN_BIT], unsigned char *key, int *MatchVector);

void makeHash(struct Name *list);

void list_add(struct Name *list, int len, unsigned char *key, unsigned char *interface);

void list_print(struct Name *list);

void Stage1_BloomFilter_print(struct BloomFilter (*list)[TEN_BIT]);

void Merged_BloomFilter_print(struct MergedBloomFilter (*list)[FORWARDING_PORT_NUM]);

void MatchVector_print(unsigned char *key, uint32_t array[]);

#endif// _OPERATION_H
