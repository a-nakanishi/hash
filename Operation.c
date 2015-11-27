#include "Operation.h"
#include "MurmurHash.h"
#include "BloomFilter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void makeFIB(struct Name *list, unsigned char *key, unsigned char *interface, uint32_t *port){
  int prefix_len = 0;
  int key_len = strlen(key);
  
  //printf("port == %u\n",*port);
  if(*port == 0){
	if(key_len > 1)
	  for(int i = 0; i < strlen(key); i++)
		if(*(key+i)=='/') prefix_len++;
  
	list_add(list, prefix_len, key, interface);  
  }
}


void LongestPrefixMatching(struct Name *list, struct BloomFilter (*bf)[TEN_BIT], unsigned char *key, int *MatchVector)
{
  int prefix_len = 0, key_len = 0;
  char tmp[NAME_PREF_LEN]="";
  uint32_t hash1, hash2, hash3;

  memset(MatchVector,0,NAME_PREF_LEN);

  for(int i = 0; i < strlen(key); i++)
    if(*(key+i)=='/') prefix_len++;

  //NamePrefix最大長時の処理
  key_len = strlen(key);
  hash1 = MurmurHash3_x86_32 (key, key_len, 1);
  hash2 = MurmurHash3_x86_32 (key, key_len, 2);
  hash3 = MurmurHash3_x86_32 (key, key_len, 3);
  checkStage1BloomFilter(bf, prefix_len, hash1, hash2, hash3, MatchVector);
  
  //Longest Prefix Matching
  for(int j = strlen(key)-1; j > 0; j--) {
	if(*(key+j)=='/'){
      strncpy(tmp,key,j);
      tmp[j] = '\0';
	  key_len = strlen(tmp);
	  hash1 = MurmurHash3_x86_32 (tmp, key_len, 1);
	  hash2 = MurmurHash3_x86_32 (tmp, key_len, 2);
	  hash3 = MurmurHash3_x86_32 (tmp, key_len, 3);
	  prefix_len--;
	  checkStage1BloomFilter(bf, prefix_len, hash1, hash2, hash3, MatchVector);
	}
  }  
}


void makeHash(struct Name *list){
  uint32_t len;
  char *tmp;
  for(int i=0; i<=NAME_PREF_LEN; i++){
    struct List *node = list[i].next;
    for(; node != NULL; node = node->next){
      len = strlen(node->NamePrefix);
      tmp = node->NamePrefix;
      node->hash1 = MurmurHash3_x86_32 (tmp, len, 1);
      node->hash2 = MurmurHash3_x86_32 (tmp, len, 2);
      node->hash3 = MurmurHash3_x86_32 (tmp, len, 3);
    }
  }
}


void makeInterface(struct MergedBloomFilter (*mbf)[MERGED_BF_SIZE], unsigned char *key, unsigned char *interface, uint32_t *port)
{
  uint32_t len, interface_num = 0, hash1, hash2, hash3;
  
  len = strlen(key);
  for(int i = 0; i < strlen(interface); i++)
	interface_num = interface_num * 10 + *(interface + i) - '0';
 
  if((0 <= interface_num) && (interface_num <= FORWARDING_PORT_NUM)){
	hash1 = MurmurHash3_x86_32 (key, len, 1);
	hash2 = MurmurHash3_x86_32 (key, len, 2);
	hash3 = MurmurHash3_x86_32 (key, len, 3);
  
	makeMergedBloomFilter(mbf, key, hash1, hash2, hash3, interface_num);
  }
  else{
	//printf("Name[%s]'s Interface %d is Out of range.\n",key,interface_num);
	*port = 1;
  }
}

void checkInterface(struct MergedBloomFilter (*mbf)[MERGED_BF_SIZE], unsigned char *key, int *MatchVector, uint32_t *default_router, uint32_t *interface_num)
{
  char tmp[NAME_PREF_BUF]="";
  int key_len, slash_count = 0;
  uint32_t hash1, hash2, hash3, Flug = 0, num = 0;
  
  memset(tmp,0,NAME_PREF_BUF);

  /*
  printf("MatchVector : ");
  for(int i=NAME_PREF_LEN-1; i>=0; i--){
	printf("%d",MatchVector[i]);
	if((i==24)||(i==16)||(i==8)) printf(" ");
  }
  printf("\n");
  */

  //printf("%s\n",key);
  for(int i=NAME_PREF_LEN; i>=1; i--){
	if(MatchVector[i-1]){
	  for(int j = 1; j <= strlen(key); j++) {
		//printf("%c\n",*(key+j));
		if((*(key+j)=='/')||(*(key+j)=='\0')){
		  slash_count++;
		  if(slash_count == i){
			strncpy(tmp,key,j);
			tmp[j] = '\0';
			key_len = strlen(tmp);
			hash1 = MurmurHash3_x86_32 (tmp, key_len, 1);
			hash2 = MurmurHash3_x86_32 (tmp, key_len, 2);
			hash3 = MurmurHash3_x86_32 (tmp, key_len, 3);
			Flug = 0;
			checkMergedBloomFilter(mbf, hash1, hash2, hash3, &Flug, &num);
			//printf("tmp = %s\n",tmp);
			//printf("num = %u\n",num);
			//printf("Flug = %u\n",Flug);
			*interface_num = num;
			if(Flug!=0) break;
		  }
		}
	  }
	  //slash_count=0;
	  if(Flug!=0){
		*default_router = 1;
		break;
	  }
	}
  }
  //printf("OK\n");
}


void list_add(struct Name *list, int len, unsigned char *key, unsigned char *interface){
  struct List *node;
  
  if((node = (struct List *)malloc(sizeof(struct List))) == NULL) {
    printf("malloc error.\n");
    exit(1);
  }
  if(list[len].next == NULL){
    strcpy(node->NamePrefix,key);
    strcpy(node->Interface,interface);
    list[len].next = node;
    node->next = NULL;
  }
  else{
    struct List *head = list[len].next;
    int Break=0;
    for(; head->next != NULL; head = head->next){
      if((strcmp(head->NamePrefix, key)) == 0) {
		Break++;
		break;
      }
    }
    if((strcmp(head->NamePrefix, key)) == 0) Break++;
    if(Break==0){
      strcpy(node->NamePrefix,key);
      strcpy(node->Interface,interface);
      head->next = node;
      node->next = NULL;
    }
  }
}


void list_print(struct Name *list){
  for(int i=0; i<=NAME_PREF_LEN; i++){
    struct List *head = list[i].next;
    for(; head != NULL; head = head->next)
      printf("Prefix %d & NextHop is [ %s ][hop: %s ][ %u ][ %u ][ %u ]\n",i,head->NamePrefix,head->Interface,head->hash1,head->hash2,head->hash3);
  }
}

void Stage1_BloomFilter_print(struct BloomFilter (*list)[TEN_BIT]){
  printf("Stage1BloomFilter:\n");
  for(int i=0; i<=DATA_SIZE; i++){
	printf("Prefix len %d ",i);
	for(int j=0; j<TEN_BIT; j++){
	  printf("%d",list[i][j].bit);
	}
	printf("\n");
  }
  printf("\n");
}

void Merged_BloomFilter_print(struct MergedBloomFilter (*list)[MERGED_BF_SIZE]){
  printf("MergedBloomFilter:\n");
  for(int i=0; i<FORWARDING_PORT_NUM; i++){
	printf("Port %d ",i);
	for(int j=0; j<MERGED_BF_SIZE; j++){
	  printf("%d",list[i][j].bit);
	}
	printf("\n");
  }
  printf("\n");
}

void MatchVector_print(unsigned char *key, uint32_t array[]){
  printf("[%s]'s MatchVector:\n",key);
  for(int i=NAME_PREF_LEN-1; i>=0; i--){
	printf("%d",array[i]);
	if((i==24)||(i==16)||(i==8)) printf(" ");
  }
  printf("\n");
}
