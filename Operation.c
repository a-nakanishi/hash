#include "Operation.h"
#include "MurmurHash.h"
#include "BloomFilter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void changeIPtoUINT(unsigned char *key, uint8_t *IP)
{
    //IP length check
    if(strlen(key) > 15) {
        printf("Error : Input IP[%s] is too long.\n", key);
        exit(1);
    }

    //IP is Correct or not check
    for(int i = 0, j = 0, k = 0, check_dotto = 0; i < strlen(key); i++) {
        if((*(key + i) == '.') && (*(key + i + 1) != '.')) {
            check_dotto++;
            IP[k++] = j;
            j = 0;
        }

        if((*(key + i) != '.') && (*(key + i) != '\0')) {
            j = j * 10 + *(key + i) - '0';

            if((j > 255) || (j < 0)) {
                printf("Error : Input IP[%s] is Out of range.\n", key);
                exit(1);
            }
        }

        if(i == (strlen(key) - 1)) {
            if(*(key + i) == '.') {
                printf("Error : Input IP[%s] is wrong.\n", key);
                exit(1);
            }

            if(check_dotto != 3) {
                printf("Error : Input IP[%s] is not exist.\n", key);
                exit(1);
            }

            IP[k] = j;
        }
    }
}


/*void splitNamePrefixAndInputBF(unsigned char *key)
{
  uint32_t hash = 0;
  int count_pref_len = 0, already = 0;
  char tmp[NAME_PREF_LEN]="";

  if(*key != '/') {//Name Input check
    printf("Error : Input Name[%s] is wrong.\n", key);
    exit(1);
  }
  for(int i = 0; i < strlen(key); i++) {

    if(*(key+i)=='/'){
      if(count_pref_len){
	strncpy(tmp,key,i);
	tmp[i]='\0';
	printf("tmp = %s\n",tmp);
	for(int j = 1; j <= 3; j++){
	  MurmurHash3_x86_32(tmp, i, j, &hash);
	  NPBF[count_pref_len][hash % TEN_BIT].bit = 1;
	  storeStage1BloomFilter( MNP, tmp, hash, count_pref_len, &already);
	}
	if(already == 0) { //FIB don't exist pattern
	  //MurmurHash3_uint32_uint32(IP_2sin, 10, (void *)&result);
	  //storeHashTable(HT, ip, hop, result, IP_2sin, pref_len);
	}
      }
      count_pref_len++;
    }

    if(((*(key+i))<33)||(126<(*(key+i)))){//||(*(key+i)=='\0')){
      strcpy(tmp,key);
      printf("tmp = %s\n",tmp);
      for(int j = 1; j <= 3; j++){
	MurmurHash3_x86_32(tmp, i, j, &hash);
	NPBF[count_pref_len][hash % TEN_BIT].bit = 1;
	storeStage1BloomFilter( MNP, tmp, hash, count_pref_len, &already);
      }
      if(already == 0) { //FIB don't exist pattern
	//MurmurHash3_uint32_uint32(IP_2sin, 10, (void *)&result);
	//storeHashTable(HT, ip, hop, result, IP_2sin, pref_len);
      }
    }
  }
  }*/

void makeFIB(struct Name *list, unsigned char *key, unsigned char *hop, int flug){
  int prefix_len = 0;
  int key_len = strlen(key);//char tmp[NAME_PREF_LEN]="";
  
  if(key_len > 1)
	for(int i = 0; i < strlen(key); i++)
	  if(*(key+i)=='/') prefix_len++;
  
  list_add(list, prefix_len, key, hop);
  
  /*
	if(flug==0){
	//printf(":Prefix is [%s]\n",key);
	for(int i = 0; i < strlen(key); i++) {
	  //printf("%d,%d,%c\n",i,*(key+i+1),*(key+i+1));
	  if(*(key+i)=='/'){
		switch(i){
		case 0 :	
		  strncpy(tmp,key,1);
		  tmp[1] = '\0';
		  break;
		default :
		  strncpy(tmp,key,i);
		  tmp[i] = '\0';
		}
		//printf(":Prefix is [%s]\n",tmp);
		list_add(list, prefix_len, tmp, hop);
		prefix_len++;
	  }
	  else if((*(key+i+1)=='\0')||(*(key+i+1)=='\n')){
		strncpy(tmp,key,i+1);
		tmp[i+1] = '\0';
		//printf(":Prefix is [%s]\n",tmp);
		list_add(list, prefix_len, tmp, hop);
		prefix_len++;
	  }
	}
	//printf("Prefix %d is [%s]\n",list->PrefixLength,list->NamePrefix);
	}*/
}

void LongestPrefixMatching(struct Name *list, struct BloomFilter (*bf)[TEN_BIT], unsigned char *key, int *MatchVector)
{
  int prefix_len = 0, key_len = 0, Flug = 0;
  char tmp[NAME_PREF_LEN]="";
  uint32_t hash1, hash2, hash3;

  memset(MatchVector,0,NAME_PREF_LEN);
  /*	for(int i=NAME_PREF_LEN-1; i>=0; i--){
	  printf("%d", MatchVector[i]);
	  if((i==8)||(i==16)||(i==24)) printf(" ");
	}
	printf("\n");*/

  for(int i = 0; i < strlen(key); i++)
    if(*(key+i)=='/') prefix_len++;

  //NamePrefix最大長時の処理
  key_len = strlen(key);
  hash1 = MurmurHash3_x86_32 (key, key_len, 1);
  hash2 = MurmurHash3_x86_32 (key, key_len, 2);
  hash3 = MurmurHash3_x86_32 (key, key_len, 3);
  //printf("through1\n");
  checkBloomFilter(bf, prefix_len, hash1, hash2, hash3, MatchVector);
  //printf("through2\n");
  
  for(int j = strlen(key)-1; j > 0; j--) {
	if(*(key+j)=='/'){
      strncpy(tmp,key,j);
      tmp[j] = '\0';
	  key_len = strlen(tmp);
	  hash1 = MurmurHash3_x86_32 (tmp, key_len, 1);
	  hash2 = MurmurHash3_x86_32 (tmp, key_len, 2);
	  hash3 = MurmurHash3_x86_32 (tmp, key_len, 3);
	  prefix_len--;
	  checkBloomFilter(bf, prefix_len, hash1, hash2, hash3, MatchVector);
	}
  }  
}

void makeHash(struct Name *list){
  uint32_t len;//,result;
  char *tmp;
  for(int i=0; i<=NAME_PREF_LEN; i++){
    struct List *node = list[i].next;
    for(; node != NULL; node = node->next){
      len = strlen(node->NamePrefix);
      tmp = node->NamePrefix;
      //strcpy(tmp, node->NamePrefix);
      //MurmurHash3_x86_32 (tmp, len, 1, result1);
      //MurmurHash3_x86_32 (tmp, len, 2, result2);
      //MurmurHash3_x86_32 (tmp, len, 3, result3);
      //result = MurmurHash3_x86_32 (tmp, len, 1);
      //printf("%s,%u\n",node->NamePrefix,result);
      node->hash1 = MurmurHash3_x86_32 (tmp, len, 1);
      node->hash2 = MurmurHash3_x86_32 (tmp, len, 2);
      node->hash3 = MurmurHash3_x86_32 (tmp, len, 3);
      //printf("%s,%u,%u,%u\n",node->NamePrefix,result1,result2,result3);
    }
    //printf("Prefix %d & NextHop is [ %s ][ %s ]\n",i,head->NamePrefix,head->NextHop);
  }

}

void list_add(struct Name *list, int len, unsigned char *key, unsigned char *hop){
  struct List *node;
  
  //printf(":Prefix %d is [%s]\n",len,key);
  if((node = (struct List *)malloc(sizeof(struct List))) == NULL) {
    printf("malloc error.\n");
    exit(1);
  }
  if(list[len].next == NULL){
    strcpy(node->NamePrefix,key);
    strcpy(node->NextHop,hop);
    list[len].next = node;
    node->next = NULL;
  }
  else{
    struct List *head = list[len].next;
    int Break=0;
    //printf("::Prefix %d is [%s]\n",len,head->NamePrefix);
    for(; head->next != NULL; head = head->next){
      if((strcmp(head->NamePrefix, key)) == 0) {
		Break++;
		break;
      }
    }
    if((strcmp(head->NamePrefix, key)) == 0) Break++;
    if(Break==0){
      strcpy(node->NamePrefix,key);
      strcpy(node->NextHop,hop);
      head->next = node;
      node->next = NULL;
    }
  }
}

/*
void list_add(struct Name *list, int len, unsigned char *key){
  struct List *node = (struct List *)malloc(sizeof(struct List));
  
  //node->PrefixLength = len;
  strcpy(node->NamePrefix,key);
  node->next = NULL;
  if(list[len].next == NULL){
    return node;
  }
  else{
    struct Name *head = list;
    for(; head != NULL; head = head->next){}
    head = node;
    return list;
  }
}*/

void list_print(struct Name *list){
  for(int i=0; i<=NAME_PREF_LEN; i++){
    struct List *head = list[i].next;
    for(; head != NULL; head = head->next)
      printf("Prefix %d & NextHop is [ %s ][hop: %s ][ %u ][ %u ][ %u ]\n",i,head->NamePrefix,head->NextHop,head->hash1,head->hash2,head->hash3);
  }
}
