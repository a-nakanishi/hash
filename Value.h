#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>

#define IP_ADDRESS_BUF 16
#define DATA_SIZE 32
#define EIGHT_BIT 256
#define TEN_BIT 16 //usage:debag
//#define TEN_BIT 1024

#define NAME_PREF_LEN 32
#define NAME_PREF_BUF 1024
#define FORWARDING_PORT_NUM 64

struct HashTable {
    struct entry *next;
} HT[DATA_SIZE + 1][TEN_BIT];

struct entry {
    unsigned char IPaddress[IP_ADDRESS_BUF];
    unsigned char NextHop[IP_ADDRESS_BUF];
    uint32_t nisin;
    struct entry *next;
};


struct BloomFilter {
    uint32_t bit;
} BF[DATA_SIZE + 1][TEN_BIT];

struct ipaddress {
    struct ip *next;
} IP[DATA_SIZE + 1][TEN_BIT];

struct ip {
    uint32_t hash;
    unsigned char IPaddress[IP_ADDRESS_BUF];
    struct ip *next;
};


struct NamePrefixBloomFilter {
    uint32_t bit;
} NPBF[NAME_PREF_LEN+1][TEN_BIT];

struct MemoryNamePrefix {
    struct NameList *next;
} MNP[NAME_PREF_LEN+1][TEN_BIT];

struct NameList {
    uint32_t hash;
    unsigned char NamePrefix[NAME_PREF_BUF];
    struct NameList *next;
};


struct Name{
  struct List *next;
} LIST[NAME_PREF_LEN+1];

struct List{
  unsigned char NamePrefix[NAME_PREF_BUF];  
  unsigned char NextHop[DATA_SIZE];  
  uint32_t hash1;
  uint32_t hash2;
  uint32_t hash3;
  struct List *next;
};



struct MergedBloomFilter {
    uint32_t bit;
} MBF[FORWARDING_PORT_NUM][TEN_BIT];

struct MergedEntry {
    struct MergedEntryList *next;
} MEL[FORWARDING_PORT_NUM][TEN_BIT];

struct MergedEntryList {
    uint32_t hash;
    unsigned char IPaddress[IP_ADDRESS_BUF];
    struct MergedEntryList *next;
};


#endif// _VALUE_H
