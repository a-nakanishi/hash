#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>
//#include <cinttypes.h>
#include <inttypes.h>

#define IP_ADDRESS_BUF 16
#define DATA_SIZE 32
#define EIGHT_BIT 256
#define TEN_BIT 16 //usage:debag
//#define TEN_BIT 1024

#define NAME_PREF_LEN 32
#define NAME_PREF_BUF 256
#define FORWARDING_PORT_NUM 32
#define MERGED_BF_SIZE 16
#define MASK_SIZE 32

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
  //uint64_t bit;
} MBF[MERGED_BF_SIZE];
//} MBF[FORWARDING_PORT_NUM][MERGED_BF_SIZE];

struct MergedEntry {
    struct MergedEntryList *next;
} MEL[FORWARDING_PORT_NUM][MERGED_BF_SIZE];

struct MergedEntryList {
  unsigned char NamePrefix[NAME_PREF_BUF];
  unsigned char Interface[DATA_SIZE];
  struct MergedEntryList *next;
};

uint32_t Mask[MASK_SIZE];/* = {
uint64_t Mask[MASK_SIZE];/* = {
  0x0000000000000001,//2^0
  0x0000000000000002,//2^1
  0x0000000000000004,//2^2
  0x0000000000000008,//2^3
  0x0000000000000010,//2^4
  0x0000000000000020,//2^5
  0x0000000000000040,//2^6
  0x0000000000000080,//2^7
  0x0000000000000100,//2^8
  0x0000000000000200,//2^9
  0x0000000000000400,//2^10
  0x0000000000000800,//2^11
  0x0000000000001000,//2^12
  0x0000000000002000,//2^13
  0x0000000000004000,//2^14
  0x0000000000008000,//2^15
  0x0000000000010000,//2^16
  0x0000000000020000,//2^17
  0x0000000000040000,//2^18
  0x0000000000080000,//2^19
  0x0000000000100000,//2^20
  0x0000000000200000,//2^21
  0x0000000000400000,//2^22
  0x0000000000800000,//2^23
  0x0000000001000000,//2^24
  0x0000000002000000,//2^25
  0x0000000004000000,//2^26
  0x0000000008000000,//2^27
  0x0000000010000000,//2^28
  0x0000000020000000,//2^29
  0x0000000040000000,//2^30
  0x0000000080000000,//2^31
  0x0000000100000000,//2^32
  0x0000000200000000,//2^33
  0x0000000400000000,//2^34
  0x0000000800000000,//2^35
  0x0000001000000000,//2^36
  0x0000002000000000,//2^37
  0x0000004000000000,//2^38
  0x0000008000000000,//2^39
  0x0000010000000000,//2^40
  0x0000020000000000,//2^41
  0x0000040000000000,//2^42
  0x0000080000000000,//2^43
  0x0000100000000000,//2^44
  0x0000200000000000,//2^45
  0x0000400000000000,//2^46
  0x0000800000000000,//2^47
  0x0001000000000000,//2^48
  0x0002000000000000,//2^49
  0x0004000000000000,//2^50
  0x0008000000000000,//2^51
  0x0010000000000000,//2^52
  0x0020000000000000,//2^53
  0x0040000000000000,//2^54
  0x0080000000000000,//2^55
  0x0100000000000000,//2^56
  0x0200000000000000,//2^57
  0x0400000000000000,//2^58
  0x0800000000000000,//2^59
  0x1000000000000000,//2^60
  0x2000000000000000,//2^61
  0x4000000000000000,//2^62
  0x8000000000000000 //2^63
};*/

#endif// _VALUE_H
