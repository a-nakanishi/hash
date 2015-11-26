#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "Value.h"

void changeIPtoUINT(unsigned char *key, uint8_t *IP);

//void splitNamePrefixAndInputBF(unsigned char *key);

void makeFIB(struct Name *list, unsigned char *key, unsigned char *interface, int flug);

//void makeInterface(struct MergedBloomFilter (*mbf)[MERGED_BF_SIZE], unsigned char *interface, int port);
void makeInterface(struct MergedBloomFilter (*mbf)[MERGED_BF_SIZE], unsigned char *key, unsigned char *interface, int port_num);

void checkInterface(struct MergedBloomFilter (*mbf)[MERGED_BF_SIZE], unsigned char *key, int *MatchVector);

void LongestPrefixMatching(struct Name *list, struct BloomFilter (*bf)[TEN_BIT], unsigned char *key, int *MatchVector);

void makeHash(struct Name *list);

//struct Name *list_add(struct Name *list, int len, unsigned char *key);
void list_add(struct Name *list, int len, unsigned char *key, unsigned char *interface);

void list_print(struct Name *list);

#endif// _OPERATION_H
