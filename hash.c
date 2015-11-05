#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define IP_ADDRESS_BUF 16
#define DATA_SIZE 32
#define EIGHT_BIT 256
#define TEN_BIT 1024
//#define TEN_BIT 10

void nisin(uint32_t x);

void MurmurHash3_x86_32(const void * key, int len, uint32_t seed, void * out);
void MurmurHash3_uint32_uint32(const uint32_t key, uint32_t seed, uint32_t *out);
/*
struct Prefix_Length{
  //uint32_t len;
  struct BloomFilter *next;
} PL[DATA_SIZE];
*/
struct BloomFilter{
  uint32_t hash;
  uint32_t bit;
  unsigned char IPaddress[IP_ADDRESS_BUF];
  struct ip *next;
} BF[DATA_SIZE+1][TEN_BIT];

struct ip{
  uint32_t hash;
  uint32_t bit;
  unsigned char IPaddress[IP_ADDRESS_BUF];
  struct ip *next;
};

void storeBloomFilter(struct BloomFilter (*bf)[TEN_BIT], unsigned char* key, uint32_t v, uint32_t size, uint32_t prefix);
//void storeBloomFilter(struct Prefix_Length *pl, unsigned char *key, uint32_t v, uint32_t size, uint32_t Prefix);
//void checkBloomFilter(struct Prefix_Length *pl, unsigned char* key, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t size);
//void storeBloomFilter(struct BloomFilter *bf, unsigned char *key, uint32_t v, uint32_t size);
//void checkBloomFilter(struct BloomFilter *bf, unsigned char* key, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t size);
void checkBloomFilter(struct BloomFilter (*bf)[TEN_BIT], unsigned char* key, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix);

//Specification : using IPv4 consideration

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
      //break;
    }
    else{
      if(store_end==0){
	//Input IP length check
	if(strlen(IP) > 18) {
	  printf("Error : Input IP-Address Plus Prefix_Length is too long.\n");
	  return 1;
	}      
	//Input IP is Correct or not check
	for(int i = 0, j = 0, k = 0; i <= strlen(IP); i++){
	  if(*(IP + i) == '.') check_colon++;
	  if(*(IP + i) == '/'){
	    IP_len=i;
	    check_slash++;
	  }
	  if(check_slash > check_colon){
	    printf("Error : Input Format is wrong.('/' appear before '.' appear)\n");
	    return 1;
	  }
	  if((*(IP + i) == '.')||(*(IP + i) == '/')){
	    if((*(IP + i + 1) != '.')||(*(IP + i + 1) != '/')){
	      IP_Address[k++]=j;
	      j = 0;
	    }
	    else if((*(IP + i + 1) == '.')||(*(IP + i + 1) == '/')){
	      printf("Error : Input Format is wrong.('/' or '.' constant)\n");
	      return 1;
	    }
	  }
	  if((*(IP + i) != '.') && (*(IP + i) != '/') && (*(IP + i) != '\0')) {
	    j = j * 10 + *(IP + i) - '0';
	    if((j > 255)||(j < 0)) {
	      printf("Error : Input IP-Address Out of range.\n");
	      return 1;
	    }
	    else if((check_colon==3)&&(check_slash==1)){
	      if((j > 32)||(j < 1)) {
		printf("Error : Input Prefix Length Out of range.\n");
		return 1;
	      }
	    }
	  }
	  if(i == (strlen(IP) - 1)){
	    if((*(IP + i) == '.')||(*(IP + i) == '/')) {
	      printf("Error : Input IP-Address is wrong. (last Input key)\n");
	      return 1;
	    }
	    if(check_colon!=3){
	      printf("Error : Input Format is wrong. (IP is wrong)\n");
	      return 1;
	    }
	    if(check_slash!=1){
	      printf("Error : Input Format is wrong. (Prefix not write)\n");
	      return 1;
	    }
	    Prefix_len=j;
	  }
	}
	//printf("IP_Address[0] = %d\n",IP_Address[0]);
	//printf("IP_Address[1] = %d\n",IP_Address[1]);
	//printf("IP_Address[2] = %d\n",IP_Address[2]);
	//printf("IP_Address[3] = %d\n",IP_Address[3]);
	//printf("Prefix_len = %d\n",Prefix_len);
	//return 0;
	
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
	  default:  printf("through\n"); break;
	  }
	  y=y/2;
	}
	/*for(int i=DATA_SIZE-1;i>DATA_SIZE-Prefix_len-1;i--){   
	  if(int_IPaddress[i]==1){
	    printf("Error : Input Format is wrong. (Prefix)\n");
	    return 1;
	  }
	}*/
	/*if(Prefix_len<32){
	  for(int i=Prefix_len;i<DATA_SIZE;i++){   
	    if(int_IPaddress[i]==1){
	      printf("Error : Input Format is wrong. (Prefix Length)\n");
	      return 1;
	    }
	  }
	}else if(Prefix_len==32){
	  for(int i=0;i<DATA_SIZE;i++){   
	    if(int_IPaddress[i]==){
	      printf("Error : Input Format is wrong. (Prefix Length)\n");
	      return 1;
	    }
	  }	
	  }*/
	
	/*
	if(Prefix_len<DATA_SIZE){
	  for(int i=DATA_SIZE-1;Prefix_len<=i;i--){   
	    if(int_IPaddress[i]==1){
	      printf("Error : Input Format is wrong. (Prefix Length)\n");
	      return 1;
	    }
	  }
	}else if(Prefix_len==DATA_SIZE){
	  if(int_IPaddress[DATA_SIZE-1]==0){
	    printf("Error : Input Format is wrong. (Prefix Length)\n");
	    return 1;
	  }	
	}
	*/
	
	*(IP+IP_len) = '\0';
	//for(int i=0;i<strlen(IP);i++) printf("IP+%d = %c\n",i,*(IP+i));
	
	//IP_2sin = IP_2sin >> Prefix_len;
	
	//usage debag : start
	/*int x=0;
	uint32_t y=IP_2sin;
	for(int i=DATA_SIZE-1;i>=0;i--){
	  x=y%2;
	  switch(x){
	  case 0:	  int_IPaddress[i]=0;  break;
	  case 1:	  int_IPaddress[i]=1;  break;
	  default:  printf("through\n"); break;
	  }
	  y=y/2;
	}
	printf("IP-Address = ");
	for(int i=0;i<32;i++){   
	  printf("%d",int_IPaddress[i]);
	  if((i==7)||(i==15)||(i==23)) printf(" ");
	}
	printf("\n");
	return 0;*/
	// usage debag : end
	
	//IP_2sin = IP_2sin >> Prefix_len;
	//printf("DATA_SIZE-Prefix_len = %d\n",DATA_SIZE-Prefix_len);

	IP_2sin = IP_2sin >> (DATA_SIZE-Prefix_len);
	uint32_t ip;
	for(uint32_t i=1;i<=3;i++){
	  //for(uint32_t j=0;j<=Prefix_len;j++){
	  //ip = IP_2sin;
	  //ip = ip >> j;
	  //  MurmurHash3_uint32_uint32(ip, i, (void *)&result);
	  MurmurHash3_uint32_uint32(IP_2sin, i, (void *)&result);
	  //storeBloomFilter(PL,IPaddress,result,TEN_BIT,Prefix_len);
	  //storeBloomFilter(BF[Prefix_len],IPaddress,result,TEN_BIT);
	  //storeBloomFilter(BF,IPaddress,result,TEN_BIT,j);
	  storeBloomFilter(BF,IPaddress,result,TEN_BIT,Prefix_len);
	  //printf("seed = %d , 32bit_hash_value = %u , hash_value/10 = %u , ",i,result,result%TEN_BIT);
	  //for(int j=0;j<TEN_BIT;j++) printf("%d",BF[j].bit);
	  //printf("\n");  
	  //}
	}
      }
      else if(store_end==1){//Check BloomFilter 
	//Input IP length check
	if(strlen(IP) > 15) {
	  printf("Error : Input IP-Address is too long.\n");
	  return 1;
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
	      printf("Error : Input IP-Address Out of range.\n");
	      return 1;
	    }
	  }
	  if(i == (strlen(IP) - 1)){
	    if(*(IP + i) == '.') {
	      printf("Error : Input IP-Address is wrong.\n");
	      return 1;
	    }
	    IP_Address[k]=j;
	  }
	}
	if(check != 3) {
	  printf("Error : Input IP-Address is not exist.\n");
	  return 1;
	}
	check=0;
	//IP_2sin = IP_Address[3]<<24 | IP_Address[2]<<16 | IP_Address[1]<<8 | IP_Address[0];
	//printf("IP Address = %u.%u.%u.%u\n",IP_Address[0],IP_Address[1],IP_Address[2],IP_Address[3]);
	IP_2sin = IP_Address[0]<<24 | IP_Address[1]<<16 | IP_Address[2]<<8 | IP_Address[3];
      	  
	uint32_t bf1,bf2,bf3,nisin=0,EXIST=0;
	for(int i=32;i>=1;i--){
	  nisin = IP_2sin>>(32-i);
	  MurmurHash3_uint32_uint32(nisin, 1, (void *)&bf1);
	  MurmurHash3_uint32_uint32(nisin, 2, (void *)&bf2);
	  MurmurHash3_uint32_uint32(nisin, 3, (void *)&bf3);
	  checkBloomFilter(BF,IPaddress,bf1,bf2,bf3,&EXIST,i);
	}
	EXIST=0;
	/*for(int i=0;i<=32;i++){
	  nisin = IP_2sin>>i;
	  MurmurHash3_uint32_uint32(nisin, 1, (void *)&bf1);
	  MurmurHash3_uint32_uint32(nisin, 2, (void *)&bf2);
	  MurmurHash3_uint32_uint32(nisin, 3, (void *)&bf3);
	  checkBloomFilter(BF,IPaddress,bf1,bf2,bf3,TEN_BIT,i);
	  }*/
      }
      /*printf("seed = 1 , 32bit_hash_value = %u , hash_value/10 = %u\n",bf1,bf1%TEN_BIT);
	printf("seed = 2 , 32bit_hash_value = %u , hash_value/10 = %u\n",bf2,bf2%TEN_BIT);
	printf("seed = 3 , 32bit_hash_value = %u , hash_value/10 = %u\n",bf3,bf3%TEN_BIT);
	printf("Build bit in BloomFilter : ");
	for(int i=0;i<TEN_BIT;i++) printf("%d",BF[i].bit);
	printf("\n");*/  
    }
  }  
  
  /*
  for(int i=0;i<TEN_BIT;i++){
    printf("%d",BF[i].bit);
    //printf("%s",BF[i].IPaddress);
  }
  printf("\n");  
  */
  /*
  for(int i=0;i<TEN_BIT;i++){
    if(BF[i].next == NULL){
      printf("i=%d : %s\n",i,BF[i].IPaddress);
    }
    else{
      printf("i=%d : %s\n",i,BF[i].IPaddress);
      struct ip *node = BF[i].next;
      printf("i=%d : %s\n",i,node->IPaddress);
      while(node->next != NULL){
	node = node->next;
	printf("i=%d : %s\n",i,node->IPaddress);
      }
      free(node);
    }
    //free(node);
  }
  printf("\n");  
  */
  return 0;
}

//void storeBloomFilter(struct BloomFilter *bf, unsigned char* key, uint32_t v, uint32_t size){
void storeBloomFilter(struct BloomFilter (*bf)[TEN_BIT], unsigned char* key, uint32_t v, uint32_t size, uint32_t prefix){
  uint32_t num = v % TEN_BIT;
  uint32_t i=0;  
  
  if(!(bf[prefix][num].bit)){//Case: not stand Flag
    bf[prefix][num].hash = v;
    bf[prefix][num].bit = 1;
    strcpy(bf[prefix][num].IPaddress,key);
    bf[prefix][num].next = NULL;
  }
  else if(strcmp(bf[prefix][num].IPaddress,key)){//Case: same IP address not already store
    struct ip *node;
    if((node= (struct ip*)malloc(sizeof(struct ip))) == NULL){
      printf("malloc error.\n");
      exit(1);
    }
    if(bf[prefix][num].next == NULL){
      node->hash = v;
      node->bit = 1;
      strcpy(node->IPaddress,key);
      bf[prefix][num].next = node;
      node->next = NULL;
    }
    else{
      struct ip *head;
      int Break=0;
      head = bf[prefix][num].next;
      while(head->next != NULL){
	if((strcmp(head->IPaddress,key))==0){
	  printf("same IP address exist in BloomFilter.\n");
	  Break++;
	  break;
	}
	head = head->next;
      }
      if(Break==0){
	node->hash = v;
	node->bit = 1;
	strcpy(node->IPaddress,key);
	head->next = node;
	node->next = NULL;
      }
    }
  }
}


//void checkBloomFilter(struct BloomFilter *bf, unsigned char* key, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t size){
void checkBloomFilter(struct BloomFilter (*bf)[TEN_BIT], unsigned char* key, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t *size, uint32_t prefix){
  uint32_t num1 = v1 % TEN_BIT;
  uint32_t num2 = v2 % TEN_BIT;
  uint32_t num3 = v3 % TEN_BIT;  
  
  if((bf[prefix][num1].bit)&&(bf[prefix][num2].bit)&&(bf[prefix][num3].bit)){//Case: check bit all stand.
    if((bf[prefix][num1].next==NULL)&&(bf[prefix][num2].next==NULL)&&(bf[prefix][num3].next==NULL)){
      if(((strcmp(bf[prefix][num1].IPaddress,key))==0)
	 &&((strcmp(bf[prefix][num2].IPaddress,key))==0)
	 &&((strcmp(bf[prefix][num3].IPaddress,key))==0)){
	printf("Prefix length = %d, IP[%s] exist in BloomFilter(exact match).\n",prefix,key);*size=1;
	//printf("%d %s exist in BloomFilter.\n",prefix,key);
      }
      else{
	printf("Prefix Length = %d, IP[%s] exist in BloomFilter(LPM).\n",prefix,key);*size=1;
	//printf("%d %s is false positve in BloomFilter.\n",prefix,key);
      }
    }
    else{
      struct ip *head1=bf[prefix][num1].next,*head2=bf[prefix][num2].next,*head3=bf[prefix][num3].next;
      int Break1=0,Break2=0,Break3=0;
      if((strcmp(bf[prefix][num1].IPaddress,key))!=0){
	for(; head1 != NULL; head1 = head1->next){
	  if((strcmp(head1->IPaddress,key))==0){
	    Break1=1;break;
	  }
	}
      }else Break1=1;
      if((strcmp(bf[prefix][num2].IPaddress,key))!=0){
	for(;head2 != NULL;head2 = head2->next){
	  if((strcmp(head2->IPaddress,key))==0){
	    Break2=1;break;
	  }
	}
      }else Break2=1;
      if((strcmp(bf[prefix][num3].IPaddress,key))!=0){
	for(;head3 != NULL;head3 = head3->next){
	  if((strcmp(head3->IPaddress,key))==0){
	    Break3=1;break;
	  }
	}
      }else Break3=1;
      if(Break1 && Break2 && Break3){
	printf("Prefix Length = %d, IP[%s] exist in BloomFilter(exact match).\n",prefix,key);*size=1;
      }else{
	printf("Prefix Length = %d, IP[%s] exist in BloomFilter(LPM).\n",prefix,key);*size=1;
	//printf("%d %s is false positve in BloomFilter.\n",prefix,key);
      }
    }
  }
  else{
    //^printf("prefix = %u\n",prefix);
    //printf("size = %d\n",*size);
    if((prefix==1)&&(*size==0)) printf("IP[%s]'s data don't exist in BloomFilter.\n",key);    
    //printf("%d %s don't exist in BloomFilter.\n",prefix,key);    
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


//test debag
void MurmurHash3_x86_32 (const void * key, int len, uint32_t seed, void * out)
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 4;

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

  for(int i = -nblocks; i; i++)
    {
      uint32_t k1 = getblock32(blocks,i);

      k1 *= c1;
      k1 = ROTL32(k1,15);
      k1 *= c2;

      h1 ^= k1;
      h1 = ROTL32(h1,13);
      h1 = h1*5+0xe6546b64;
    }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;

  switch(len & 3)
    {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];
      k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
    };

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  *(uint32_t*)out = h1;
}

// void MurmurHash3_uint32_uint32(const void * key, int len, uint32_t seed, void * out)
//32bit
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
