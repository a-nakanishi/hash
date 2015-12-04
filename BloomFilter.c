
#include "BloomFilter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void storeStage1BloomFilter(struct Name *list, struct BloomFilter (*bf)[TEN_BIT])
{
  struct List *node;
  uint32_t num1, num2, num3;

  for(int i=0; i<=NAME_PREF_LEN; i++){
    if(list[i].next != NULL){
      if((node = (struct List *)malloc(sizeof(struct List))) == NULL) {
		printf("malloc error.\n");
		exit(1);
      }
      node = list[i].next;
      for(; node != NULL; node = node->next){
		num1 = node->hash1 % TEN_BIT;
		num2 = node->hash2 % TEN_BIT;
		num3 = node->hash3 % TEN_BIT;
        bf[i][num1].bit++;
        bf[i][num2].bit++;
        bf[i][num3].bit++;
      }
    }
  }
}    


void checkStage1BloomFilter(struct BloomFilter (*bf)[TEN_BIT], int pref_len, uint32_t num1, uint32_t num2, uint32_t num3, int *MatchVector){  
  if((bf[pref_len][num1 % TEN_BIT].bit) && (bf[pref_len][num2 % TEN_BIT].bit) && (bf[pref_len][num3 % TEN_BIT].bit)) {
	MatchVector[pref_len - 1] = 1;
  } else {
	MatchVector[pref_len - 1] = 0;
  }  
}


void makeMergedBloomFilter(struct MergedBloomFilter (*mbf)[FORWARDING_PORT_NUM], unsigned char *key, uint32_t hash1, uint32_t hash2, uint32_t hash3, int port_num)
{
  uint32_t buf[FORWARDING_PORT_NUM][MERGED_BF_SIZE] = {};

  buf[port_num][hash1 % MERGED_BF_SIZE] = 1;
  buf[port_num][hash2 % MERGED_BF_SIZE] = 1;
  buf[port_num][hash3 % MERGED_BF_SIZE] = 1;

  for(int a=0; a<MERGED_BF_SIZE; a++)
	for(int b=0; b<FORWARDING_PORT_NUM; b++)
	  mbf[a][b].bit |= buf[b][a];    
}

void checkMergedBloomFilter(struct MergedBloomFilter (*mbf)[FORWARDING_PORT_NUM], uint32_t num1, uint32_t num2, uint32_t num3, uint32_t *flug, uint32_t *interface_num){  
  uint32_t result[FORWARDING_PORT_NUM] = {};
  for(int a=0; a<FORWARDING_PORT_NUM; a++){
	result[a] = 
	  mbf[num1 % MERGED_BF_SIZE][a].bit &
	  mbf[num2 % MERGED_BF_SIZE][a].bit & 
	  mbf[num3 % MERGED_BF_SIZE][a].bit;  
	if(result[a]){
	  *interface_num = a;
	  *flug = 1;
	}
  }
}
