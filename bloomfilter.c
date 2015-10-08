#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define IP_ADDRESS_BUF 16
#define DATA_SIZE 32
#define EIGHT_BIT 256
#define TEN_BIT 1024

void nisin(uint32_t x);
void MurmurHash3_uint32_uint32(const uint32_t key, uint32_t seed, uint32_t *out);

struct HashTable{
  struct entry *next;  
} HT[DATA_SIZE+1][TEN_BIT];

struct entry{
  unsigned char IPaddress[IP_ADDRESS_BUF];
  unsigned char NextHop[8];
  struct entry *next;
};

struct BloomFilter{
  uint32_t bit;
} BF[DATA_SIZE+1][TEN_BIT];

struct ipaddress{
  //uint32_t hash;
  //unsigned char IPaddress[IP_ADDRESS_BUF];
  struct ip *next;
} IPADDRESS[DATA_SIZE+1][TEN_BIT];

struct ip{
  uint32_t hash;
  unsigned char IPaddress[IP_ADDRESS_BUF];
  struct ip *next;
};

void storeBloomFilter(struct ipaddress (*ip)[TEN_BIT], unsigned char* key, uint32_t hash, uint32_t *size, uint32_t prefix);
void checkBloomFilter(struct ipaddress (*ip)[TEN_BIT], struct BloomFilter (*bf)[TEN_BIT], unsigned char* key, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix, int *matchvector);
void makeHashTable(struct HashTable (*bit)[TEN_BIT], unsigned char* key, uint32_t hash,  uint32_t prefix);


int main(int argc, char *argv[])
{
  unsigned char  IPaddress[IP_ADDRESS_BUF], *IP;
  unsigned int   hash, seed = 1;
  
  uint8_t  IP_Address[4]={};
  uint32_t IP_len = 0, Prefix_len = 0;
  uint32_t result = 0, IP_2sin = 0;
  int check_colon = 0;
  int check_slash = 0;
  int check = 0;
  int store_end = 0;
  
  int int_IPaddress[DATA_SIZE];
  uint8_t IP_buf;
  
  int MatchVector[DATA_SIZE];
  
  for(int loop=0;;loop++){
    printf("Input IPv4-Address : ");
    if(loop==0) scanf("%[1234567890./]", IPaddress);
    else scanf("%*c %[1234567890./]", IPaddress);
    IP = &IPaddress[0];
    //End store & check BloomFilter is input '0'.     
    if((strlen(IP)==1)&&(*IP=='0')){
      if(store_end==0){
	printf("IP address has cashed in BloomFilter.\n");
	printf("Next: check IP address exist or not in BloomFilter.\n");
      }else{
	printf("End to IP address check in BloomFilter.\n");
	break;
      }
      store_end++;
    }
    else{
      if(store_end==0){
	//Input IP length check
	if(strlen(IP) > 18) {
	  printf("Error : Input IP-Address Plus Prefix_Length is too long.\n");return 1;
	}      
	//Input IP is Correct or not check
	for(int i = 0, j = 0, k = 0; i <= strlen(IP); i++){
	  if(*(IP + i) == '.') check_colon++;
	  if(*(IP + i) == '/'){
	    IP_len=i;
	    check_slash++;
	  }
	  if(check_slash > check_colon){
	    printf("Error : Input Format is wrong.('/' appear before '.' appear)\n");return 1;
	  }
	  if((*(IP + i) == '.')||(*(IP + i) == '/')){
	    if((*(IP + i + 1) != '.')||(*(IP + i + 1) != '/')){
	      IP_Address[k++]=j;
	      j = 0;
	    }
	    else if((*(IP + i + 1) == '.')||(*(IP + i + 1) == '/')){
	      printf("Error : Input Format is wrong.('/' or '.' constant)\n"); return 1;
	    }
	  }
	  if((*(IP + i) != '.') && (*(IP + i) != '/') && (*(IP + i) != '\0')) {
	    j = j * 10 + *(IP + i) - '0';
	    if((j > 255)||(j < 0)) {
	      printf("Error : Input IP-Address Out of range.\n");return 1;
	    }
	    else if((check_colon==3)&&(check_slash==1)){
	      if((j > 32)||(j < 1)) {
		printf("Error : Input Prefix Length Out of range.\n");return 1;
	      }
	    }
	  }
	  if(i == (strlen(IP) - 1)){
	    if((*(IP + i) == '.')||(*(IP + i) == '/')) {
	      printf("Error : Input IP-Address is wrong. (last Input key)\n");return 1;
	    }
	    if(check_colon!=3){
	      printf("Error : Input Format is wrong. (IP is wrong)\n");return 1;
	    }
	    if(check_slash!=1){
	      printf("Error : Input Format is wrong. (Prefix not write)\n");return 1;
	    }
	    Prefix_len=j;
	  }
	}	
	check_colon=0;
	check_slash=0;
	
	IP_2sin = IP_Address[0]<<24 | IP_Address[1]<<16 | IP_Address[2]<<8 | IP_Address[3];
	int x=0;
	uint32_t y=IP_2sin;
	for(int i=DATA_SIZE-1;i>=0;i--){
	  x=y%2;
	  switch(x){
	  case 0:	  int_IPaddress[i]=0;  break;
	  case 1:	  int_IPaddress[i]=1;  break;
	  default:        printf("through\n"); break;
	  }
	  y=y/2;
	}
		
	*(IP+IP_len) = '\0';
	IP_2sin = IP_2sin >> (DATA_SIZE-Prefix_len);
	uint32_t ALREADY=0;
	for(uint32_t i=1;i<=3;i++){
	  MurmurHash3_uint32_uint32(IP_2sin, i, (void *)&result);
	  BF[Prefix_len][result%TEN_BIT].bit = 1;
	  storeBloomFilter(IPADDRESS,IPaddress,result,&ALREADY,Prefix_len);
	}
	ALREADY=0;
      }
      else if(store_end==1){//Check BloomFilter 
	//Input IP length check
	if(strlen(IP) > 15) {
	  printf("Error : Input IP-Address is too long.\n");return 1;
	}      
	//Input IP is Correct or not check
	for(int i = 0, j = 0, k = 0; i <= strlen(IP); i++) {
	  if((*(IP + i) == '.') && (*(IP + i + 1) != '.')) {
	    check++;
	    IP_Address[k++]=j;
	    j = 0;
	  }
	  if((*(IP + i) != '.') && (*(IP + i) != '\0')) {
	    j = j * 10 + *(IP + i) - '0';	
	    if((j > 255)||(j < 0)) {
	      printf("Error : Input IP-Address Out of range.\n");return 1;
	    }
	  }
	  if(i == (strlen(IP) - 1)){
	    if(*(IP + i) == '.') {
	      printf("Error : Input IP-Address is wrong.\n");return 1;
	    }
	    IP_Address[k]=j;
	  }
	}
	if(check != 3) {
	  printf("Error : Input IP-Address is not exist.\n");return 1;
	}
	check=0;
	IP_2sin = IP_Address[0]<<24 | IP_Address[1]<<16 | IP_Address[2]<<8 | IP_Address[3];
	uint32_t ip1,ip2,ip3,nisin=0,EXIST=0;
	for(int i=32;i>=1;i--){
	  nisin = IP_2sin>>(32-i);
	  MurmurHash3_uint32_uint32(nisin, 1, (void *)&ip1);
	  MurmurHash3_uint32_uint32(nisin, 2, (void *)&ip2);
	  MurmurHash3_uint32_uint32(nisin, 3, (void *)&ip3);
	  checkBloomFilter(IPADDRESS,BF,IPaddress,ip1,ip2,ip3,&EXIST,i,MatchVector);
	}
	printf("Match Vector = ");
	for(int i=31;i>=0;i--){   
	  printf("%d",MatchVector[i]);
	  if((i==8)||(i==16)||(i==24)) printf(" ");
	}
	printf("\n");
	EXIST=0;
      }
    }
  }  
  return 0;
}

void storeBloomFilter(struct ipaddress (*ip)[TEN_BIT], unsigned char* key, uint32_t hash, uint32_t *size, uint32_t prefix){
  uint32_t num = hash % TEN_BIT;
  //uint32_t i=0;  
  struct ip *node;
  
  if(ip[prefix][num].next == NULL){//Case: not stand bit
    if((node= (struct ip*)malloc(sizeof(struct ip))) == NULL){
      printf("malloc error.\n"); exit(1);
    }
    node->hash = hash;
    strcpy(node->IPaddress,key);
    ip[prefix][num].next = node;
    node->next = NULL;
  }
  else{
    struct ip *head;
    int Break=0;
    head = ip[prefix][num].next;
    for(; head != NULL; head = head->next){
      if((strcmp(head->IPaddress,key))==0){
	if(*size == 0) printf("same IP address exist in BloomFilter.\n");
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

void checkBloomFilter(struct ipaddress (*ip)[TEN_BIT], struct BloomFilter (*bf)[TEN_BIT], unsigned char* key, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix, int *matchvector){
  uint32_t num1 = v1 % TEN_BIT, num2 = v2 % TEN_BIT, num3 = v3 % TEN_BIT;  
  struct ip *node1=ip[prefix][num1].next, *node2=ip[prefix][num2].next, *node3=ip[prefix][num3].next;
  
  if((bf[prefix][num1].bit)&&(bf[prefix][num2].bit)&&(bf[prefix][num3].bit)){
    if((ip[prefix][num1].next==NULL)&&(ip[prefix][num2].next==NULL)&&(ip[prefix][num3].next==NULL)){
      if(((strcmp(node1->IPaddress,key))==0)
	 &&((strcmp(node2->IPaddress,key))==0)
	 &&((strcmp(node3->IPaddress,key))==0)){
	//printf("Prefix length = %d, IP[%s] exist in BloomFilter(exact match).\n",prefix,key);
	//printf("1");
	matchvector[prefix-1] = 1;
	*size=1;
      }
      else{
	//printf("Prefix Length = %d, IP[%s] exist in BloomFilter(false positve or LPM).\n",prefix,key);
	//printf("1");
	matchvector[prefix-1] = 1;
	*size=1;
      }
    }
    else{
      int Break1=0,Break2=0,Break3=0;
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
	//printf("Prefix Length = %d, IP[%s] exist in BloomFilter(exact match).\n",prefix,key);*size=1;
	matchvector[prefix-1] = 1;
	//printf("1");
      }else{
	//printf("Prefix Length = %d, IP[%s] exist in BloomFilter(LPM).\n",prefix,key);*size=1;
	matchvector[prefix-1] = 1;
	//printf("1");
      }
    }
  }
  else{
    //if((prefix==1)&&(*size==0)) printf("IP[%s]'s data don't exist in BloomFilter.\n",key);    
    matchvector[prefix-1] = 0;
    //printf("0");
  }
  //if((prefix==8)||(prefix==16)||(prefix==24)) printf(" ");
}
  

void makeHashTable(struct HashTable (*bit)[TEN_BIT], unsigned char* key, uint32_t hash,  uint32_t prefix){
  uint32_t num = hash % TEN_BIT;
  // uint32_t i=0;  
  struct entry *node;
  
  if(HT[prefix][num].next == NULL){//Case: not hashing
    if((node= (struct entry*)malloc(sizeof(struct entry))) == NULL){
      printf("malloc error.\n"); exit(1);
    }
    //node->hash = hash;
    strcpy(node->IPaddress,key);
    HT[prefix][num].next = node;
    node->next = NULL;
  }
  else{
    struct entry *head;
    int Break=0;
    head = HT[prefix][num].next;
    for(; head != NULL; head = head->next){
      if((strcmp(head->IPaddress,key))==0){
	if(*size == 0) printf("same IP address exist in BloomFilter.\n");
	*size=1;Break++;break;
      }
    }
    if(Break==0){
      if((node= (struct entry*)malloc(sizeof(struct entry))) == NULL){
	printf("malloc error.\n"); exit(1);
      }
      //node->hash = hash;
      strcpy(node->IPaddress,key);
      head = node;
      node->next = NULL;
    }
  }  
}


uint32_t getblock32 ( const uint32_t * p, int i ){
  return p[i];
}

uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}
#define	ROTL32(x,y)	rotl32(x,y)


void MurmurHash3_uint32_uint32(const uint32_t key, uint32_t seed, uint32_t *out)
{
  uint32_t h1 = seed;

  const uint32_t len = 4;
  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  // body
  uint32_t k1 = key;

  k1 *= c1;
  k1 = ROTL32(k1,15);
  k1 *= c2;

  h1 ^= k1;
  h1 = ROTL32(h1,13);
  h1 = h1*5+0xe6546b64;

  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  *(uint32_t*)out = h1;
}

 void nisin(uint32_t x){
   unsigned int k[DATA_SIZE]={}, a=x;
  for(int i=DATA_SIZE;0<i;i--){
    unsigned int j = a%2;
    switch(j){
    case 0:	  k[i-1]=0;  break;
    case 1:	  k[i-1]=1;  break;
    default:  printf("through\n"); break;
    }
    a=a/2;    
  }
  printf("hash bit = ");
  for(int i=0;i<DATA_SIZE;i++){   
    printf("%d",k[i]);
    if((i==7)||(i==15)||(i==23)) printf(" ");
  }
  printf("\n");
}
