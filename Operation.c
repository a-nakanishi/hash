#include "Operation.h"

void changeIPtoUINT(unsigned char* key, uint8_t *IP){
  //IP length check
  if(strlen(key) > 15) {
    printf("Error : Input IP[%s] is too long.\n",key);
    exit(1);
  }
  //IP is Correct or not check
  for(int i=0, j=0, k=0, check_dotto=0; i<strlen(key); i++) {
    //printf("*key+%d = %d\n",i,*(key+i));
    if((*(key + i) == '.') && (*(key + i + 1) != '.')) {
      check_dotto++;
      IP[k++]=j;
      j = 0;
    }
    if((*(key + i) != '.') && (*(key + i) != '\0')) {
      j = j * 10 + *(key + i) - '0';	
      if((j > 255)||(j < 0)) {
	printf("Error : Input IP[%s] is Out of range.\n",key);
	exit(1);
      }
    }
    if(i == (strlen(key) - 1)){
      if(*(key + i) == '.') {
	printf("Error : Input IP[%s] is wrong.\n",key);
	exit(1);
      }
      if(check_dotto != 3) {
	printf("Error : Input IP[%s] is not exist.\n",key);
	exit(1);
      }
      IP[k]=j;
    }
  }
}
