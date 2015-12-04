#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>

#define IP_ADDRESS_BUF 16
#define DATA_SIZE 32
#define EIGHT_BIT 256
#define TEN_BIT 16 //usage:debag
//#define TEN_BIT 1024

#define NAME_PREF_LEN 32
#define NAME_PREF_BUF 256
#define FORWARDING_PORT_NUM 48
#define MERGED_BF_SIZE 16

struct BloomFilter {
    uint32_t bit;
} BF[DATA_SIZE + 1][TEN_BIT];


struct Name{
  struct List *next;
} LIST[NAME_PREF_LEN+1];

struct List{
  unsigned char NamePrefix[NAME_PREF_BUF];  
  unsigned char Interface[DATA_SIZE];  
  uint32_t hash1;
  uint32_t hash2;
  uint32_t hash3;
  struct List *next;
};


struct MergedBloomFilter {
    uint32_t bit;
} MBF[MERGED_BF_SIZE][FORWARDING_PORT_NUM];
//} MBF[FORWARDING_PORT_NUM][MERGED_BF_SIZE];

struct MergedEntry {
    struct MergedEntryList *next;
} MEL[FORWARDING_PORT_NUM][MERGED_BF_SIZE];

struct MergedEntryList {
  unsigned char NamePrefix[NAME_PREF_BUF];
  unsigned char Interface[DATA_SIZE];
  struct MergedEntryList *next;
};


#endif// _VALUE_H
