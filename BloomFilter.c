#include "BloomFilter.h"

void storeBloomFilter(struct ipaddress (*ip)[TEN_BIT], unsigned char* key, uint32_t hash, uint32_t *size, uint32_t prefix){
  uint32_t num = hash % TEN_BIT;
  struct ip *node,*head;
  int Break=0;
  
  if(ip[prefix][num].next == NULL){//Case: not stand bit
    if((node= (struct ip*)malloc(sizeof(struct ip))) == NULL){
      printf("malloc error.\n"); exit(1);
    }
    node->hash = hash;
    strcpy(node->IPaddress,key);
    ip[prefix][num].next = node;
    node->next = NULL;
  }
  else{//Case: already bit stand
    head = ip[prefix][num].next;
    for(; head != NULL; head = head->next){
      if((strcmp(head->IPaddress,key))==0){
	//if(*size == 0) printf("same IP address exist in BloomFilter.\n");
	*size=1;Break++;break;
      }
    }
    if(Break==0){
      if((node= (struct ip*)malloc(sizeof(struct ip))) == NULL){
	printf("malloc error.\n"); exit(1);
      }
      node->hash = hash;
      strcpy(node->IPaddress,key);
      head = node;
      node->next = NULL;
    }
  }
}


void checkBloomFilter(//struct ipaddress (*ip)[TEN_BIT], 
		      struct BloomFilter (*bf)[TEN_BIT], //unsigned char* key, 
		      uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix, int *matchvector){
  uint32_t num1 = v1 % TEN_BIT, num2 = v2 % TEN_BIT, num3 = v3 % TEN_BIT;  
  
  if((bf[prefix][num1].bit)&&(bf[prefix][num2].bit)&&(bf[prefix][num3].bit)){
    matchvector[prefix-1] = 1;
  }
  else{
    matchvector[prefix-1] = 0;
  }
}


void deleteBloomFilter(struct ipaddress (*ip)[TEN_BIT],
		       struct BloomFilter (*bf)[TEN_BIT],unsigned char* key,
		       uint32_t hash, uint32_t v1, uint32_t v2, uint32_t v3,
		       uint32_t *size, uint32_t prefix){
  uint32_t num = hash % TEN_BIT, num1 = v1 % TEN_BIT, num2 = v2 % TEN_BIT, num3 = v3 % TEN_BIT;  
  struct ip *node,*head;
  int Break=0;
    
  if(ip[prefix][num].next == NULL){//Case: not stand bit
    //not operation
  }
  else{//Case: already bit stand
    head = ip[prefix][num].next;
    for(; head != NULL; head = head->next){
      if((strcmp(head->IPaddress,key))==0){
	//if(*size == 0) printf("same IP address exist in BloomFilter.\n");
	node = head;
	*size=1;
	head = ip[prefix][num].next;
	for(; head != NULL; head = head->next){
	  if((head->next)==node){
	    head->next = node->next;
	    free(node);
	    break;
	  }
	}
	//Case : Counting BloomFilter not consider
	bf[prefix][num1].bit = 0;
	bf[prefix][num2].bit = 0;
	bf[prefix][num3].bit = 0;
	break;
      }
    }
  }
}


void checkFalsePositive(struct ipaddress (*ip)[TEN_BIT], struct BloomFilter (*bf)[TEN_BIT], unsigned char* key, 
			uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix, int *matchvector){
  uint32_t num1 = v1 % TEN_BIT, num2 = v2 % TEN_BIT, num3 = v3 % TEN_BIT;  
  struct ip *node1=ip[prefix][num1].next, *node2=ip[prefix][num2].next, *node3=ip[prefix][num3].next;  
  int Break1=0,Break2=0,Break3=0;
  
  if((ip[prefix][num1].next==NULL)&&(ip[prefix][num2].next==NULL)&&(ip[prefix][num3].next==NULL)){
    if(((strcmp(node1->IPaddress,key))==0)&&((strcmp(node2->IPaddress,key))==0)&&((strcmp(node3->IPaddress,key))==0)){
      printf("when Prefix length = %u, normal operation.\n",prefix);
    }
    else{
      printf("when Prefix length = %u, occur false positve.\n",prefix);
    }
  }
  else{
    if((strcmp(node1->IPaddress,key))!=0){
      for(; node1 != NULL; node1 = node1->next){
	if((strcmp(node1->IPaddress,key))==0){
	  Break1=1;break;
	}
      }
    }else Break1=1;
    if((strcmp(node2->IPaddress,key))!=0){
      for(; node2 != NULL; node2 = node2->next){
	if((strcmp(node2->IPaddress,key))==0){
	  Break2=1;break;
	}
      }
    }else Break2=1;
    if((strcmp(node3->IPaddress,key))!=0){
      for(; node3 != NULL; node3 = node3->next){
	if((strcmp(node3->IPaddress,key))==0){
	  Break3=1;break;
	}
      }
    }else Break3=1;
    if(Break1 && Break2 && Break3){
      printf("when Prefix length = %u, normal operation.\n",prefix);
    }
    else{
      printf("when Prefix length = %u, occur false positve.\n",prefix);
    }
  }
}
