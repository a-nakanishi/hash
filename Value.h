#ifndef _VALUE_H_
#define _VALUE_H_

#define IP_ADDRESS_BUF 16
#define DATA_SIZE 32
#define EIGHT_BIT 256
//#define TEN_BIT 10
#define TEN_BIT 1024

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct HashTable{
  struct entry *next;  
} HT[DATA_SIZE+1][TEN_BIT];


struct entry{
  unsigned char IPaddress[IP_ADDRESS_BUF];
  unsigned char NextHop[IP_ADDRESS_BUF];
  uint32_t nisin;
  struct entry *next;
};


struct BloomFilter{
  uint32_t bit;
} BF[DATA_SIZE+1][TEN_BIT];


struct ipaddress{
  struct ip *next;
} IP[DATA_SIZE+1][TEN_BIT];


struct ip{
  uint32_t hash;
  unsigned char IPaddress[IP_ADDRESS_BUF];
  struct ip *next;
};

#endif// _VALUE_H
