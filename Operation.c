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


void makeInterface(struct MergedBloomFilter *mbf, unsigned char *key, unsigned char *interface, uint32_t *port)
{
  uint32_t len, interface_num = 0, hash1, hash2, hash3;
  
  len = strlen(key);
  for(int i = 0; i < strlen(interface); i++)
	interface_num = interface_num * 10 + *(interface + i) - '0';
 
  if((0 <= interface_num) && (interface_num < FORWARDING_PORT_NUM)){
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

void checkInterface(struct MergedBloomFilter *mbf, unsigned char *key, int *MatchVector, int *default_router, uint32_t *interface_num)
{
  char tmp[NAME_PREF_BUF]="";
  int key_len, slash_count = 0;
  uint32_t hash1, hash2, hash3;//, Flug = 0, num = 0;
  int Flug = 0, num = 0;
  
  memset(tmp,0,NAME_PREF_BUF);

  /*
  printf("MatchVector : ");
  for(int i=NAME_PREF_LEN-1; i>=0; i--){
	printf("%d",MatchVector[i]);
	if((i==24)||(i==16)||(i==8)) printf(" ");
  }
  printf("\n");
  */
  
  printf("---%s---\n",key);
  for(int i=NAME_PREF_LEN; i>=1; i--){
	if(MatchVector[i-1]){
	  for(int j = 1; j <= strlen(key); j++) {
		//printf("%c\n",*(key+j));
		if((*(key+j)=='/')||(*(key+j)=='\0')){
		  slash_count++;
		  if(slash_count == i){
			strncpy(tmp,key,j);
			tmp[j] = '\0';
			printf("%s : ",tmp);
			key_len = strlen(tmp);
			hash1 = MurmurHash3_x86_32 (tmp, key_len, 1);
			hash2 = MurmurHash3_x86_32 (tmp, key_len, 2);
			hash3 = MurmurHash3_x86_32 (tmp, key_len, 3);
			//Flug = 0;
			checkMergedBloomFilter(mbf, hash1, hash2, hash3, &Flug, &num);
			//printf("tmp = %s\n",tmp);
			//printf("num = %u\n",num);
			//printf("Flug = %u\n",Flug);
			*interface_num = num;
			//if(Flug==1) break;
			//if((Flug==1)||(Flug==2)) break;
			if(Flug!=0) break;
		  }
		}
	  }
	  //slash_count=0;
	  if(Flug==1){
		*default_router = 1;
		break;
	  }
	}
  }
  if(Flug==2) *default_router = 2;
  
  printf("\n");
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


//void Mask_Memory(uint64_t *mask){
void Mask_Memory(uint32_t *mask){
  mask[0] = 0x0000000000000001;//2^0
  mask[1] = 0x0000000000000002;//2^1
  mask[2] = 0x0000000000000004;//2^2
  mask[3] = 0x0000000000000008;//2^3
  mask[4] = 0x0000000000000010;//2^4
  mask[5] = 0x0000000000000020;//2^5
  mask[6] = 0x0000000000000040;//2^6
  mask[7] = 0x0000000000000080;//2^7
  mask[8] = 0x0000000000000100;//2^8
  mask[9] = 0x0000000000000200;//2^9
  mask[10] = 0x0000000000000400;//2^10
  mask[11] = 0x0000000000000800;//2^11
  mask[12] = 0x0000000000001000;//2^12
  mask[13] = 0x0000000000002000;//2^13
  mask[14] = 0x0000000000004000;//2^14
  mask[15] = 0x0000000000008000;//2^15
  mask[16] = 0x0000000000010000;//2^16
  mask[17] = 0x0000000000020000;//2^17
  mask[18] = 0x0000000000040000;//2^18
  mask[19] = 0x0000000000080000;//2^19
  mask[20] = 0x0000000000100000;//2^20
  mask[21] = 0x0000000000200000;//2^21
  mask[22] = 0x0000000000400000;//2^22
  mask[23] = 0x0000000000800000;//2^23
  mask[24] = 0x0000000001000000;//2^24
  mask[25] = 0x0000000002000000;//2^25
  mask[26] = 0x0000000004000000;//2^26
  mask[27] = 0x0000000008000000;//2^27
  mask[28] = 0x0000000010000000;//2^28
  mask[29] = 0x0000000020000000;//2^29
  mask[30] = 0x0000000040000000;//2^30
  mask[31] = 0x0000000080000000;//2^31
  //mask[32] = 0x0000000100000000;//2^32
  //mask[33] = 0x0000000200000000;//2^33
  //mask[34] = 0x0000000400000000;//2^34
  //mask[35] = 0x0000000800000000;//2^35
  //mask[36] = 0x0000001000000000;//2^36
  //mask[37] = 0x0000002000000000;//2^37
  //mask[38] = 0x0000004000000000;//2^38
  //mask[39] = 0x0000008000000000;//2^39
  //mask[40] = 0x0000010000000000;//2^40
  //mask[41] = 0x0000020000000000;//2^41
  //mask[42] = 0x0000040000000000;//2^42
  //mask[43] = 0x0000080000000000;//2^43
  //mask[44] = 0x0000100000000000;//2^44
  //mask[45] = 0x0000200000000000;//2^45
  //mask[46] = 0x0000400000000000;//2^46
  //mask[47] = 0x0000800000000000;//2^47
  //mask[48] = 0x0001000000000000;//2^48
  //mask[49] = 0x0002000000000000;//2^49
  //mask[50] = 0x0004000000000000;//2^50
  //mask[51] = 0x0008000000000000;//2^51
  //mask[52] = 0x0010000000000000;//2^52
  //mask[53] = 0x0020000000000000;//2^53
  //mask[54] = 0x0040000000000000;//2^54
  //mask[55] = 0x0080000000000000;//2^55
  //mask[56] = 0x0100000000000000;//2^56
  //mask[57] = 0x0200000000000000;//2^57
  //mask[58] = 0x0400000000000000;//2^58
  //mask[59] = 0x0800000000000000;//2^59
  //mask[60] = 0x1000000000000000;//2^60
  //mask[61] = 0x2000000000000000;//2^61
  //mask[62] = 0x4000000000000000;//2^62
  //mask[63] = 0x8000000000000000; //2^63
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

void Merged_BloomFilter_print(struct MergedBloomFilter *list){
  printf("MergedBloomFilter:\n");
  //for(int i=0; i<MERGED_BF_SIZE; i++)
  //	printf("%d : %llu\n",i,list[i].bit); 
}

void MatchVector_print(unsigned char *key, uint32_t array[]){
  printf("[%s]'s MatchVector:\n",key);
  for(int i=NAME_PREF_LEN-1; i>=0; i--){
	printf("%d",array[i]);
	if((i==24)||(i==16)||(i==8)) printf(" ");
  }
  printf("\n");
}
