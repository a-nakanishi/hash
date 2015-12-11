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


void makeMergedBloomFilter(struct MergedBloomFilter *mbf, unsigned char *key, uint32_t hash1, uint32_t hash2, uint32_t hash3, int port_num)
{
  //uint64_t bit = 1;
  uint32_t bit = 1;
  //printf("%u\n",bitport_num);

  mbf[hash1 % MERGED_BF_SIZE].bit |= bit << port_num;
  mbf[hash2 % MERGED_BF_SIZE].bit |= bit << port_num;
  mbf[hash3 % MERGED_BF_SIZE].bit |= bit << port_num;
  //printf("%u\n",mbf[hash1 % MERGED_BF_SIZE].bit);
}

void checkMergedBloomFilter(struct MergedBloomFilter *mbf, uint32_t num1, uint32_t num2, uint32_t num3, int *flug, int *interface_num){
  uint32_t result;

  result = mbf[num1 % MERGED_BF_SIZE].bit & mbf[num2 % MERGED_BF_SIZE].bit & mbf[num3 % MERGED_BF_SIZE].bit;  
  //for(int i=0;i<MERGED_BF_SIZE;i++)
  //	printf("mbf[%d] = %u\n",i+1,mbf[i].bit);
  /*for(uint32_t i=0; i<FORWARDING_PORT_NUM; i++){
  	if(result & Mask[i]) printf("%u\n",i);
	}*/
  if(result){
	int case_check = 0;
	printf("result = %u\n",result);
	for(int i=0; ; i++){
	  printf("flug = %d, %u\n",*flug,Mask[i]);
	  //if((flug == 0) && (result == Mask[i])){//port only-one exist case
	  if((case_check == 0) && (result == Mask[i])){//port only-one exist case
		//printf("%u\n",Mask[i]);
		*interface_num = i;
		//printf("interface = %u\n",*interface_num);
		case_check = 1;
		//*flug = 1;
	  }
	  else if((result != Mask[i]) && (result & Mask[i]) && (case_check != 1)){//occur false positive
		//if((result != Mask[i]) && (result & Mask[i]) && (*flug != 1)){//occur false positive
		*interface_num |= 1 << i;
		//printf("|%d|",i);
		//*interface_num = i;
		case_check = 2;
		//*flug = 2;
	  }
	  //if(*flug) printf("%d ",i);
	  //if((i >= FORWARDING_PORT_NUM)||(*flug == 1)){
	  //if((i >= MERGED_BF_SIZE)||(case_check == 1)){
	  if((i >= FORWARDING_PORT_NUM)||(case_check == 1)){
		//printf("ok");
		break;
	  }
	}
	//printf("%d\n",case_check);
	if(case_check == 1) *flug = 1;
	if(case_check == 2) *flug = 2;
	//printf("\n");
	//printf("flug = %d\n",*flug);
	//printf("%PRLu32",interface_num);
	/*for(int i=0; ; i++){
	  if(result & Mask[i]){
		*interface_num = i;
		*flug = 1;
		break;
	  }
	  if(i >= FORWARDING_PORT_NUM) break;*/
  }
}
