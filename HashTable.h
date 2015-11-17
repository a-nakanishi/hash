#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "Value.h"

void storeHashTable(struct HashTable(*ht)[TEN_BIT], unsigned char *key, unsigned char *hop, uint32_t hash, uint32_t ip_2sin, uint32_t prefix);

void checkHashTable(struct HashTable(*ht)[TEN_BIT], unsigned char *key, uint32_t hash, uint32_t ip_2sin, uint32_t prefix, uint32_t *comp, unsigned char **hop);

#endif// _HASHTABLE_H
