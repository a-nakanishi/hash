#include "BloomFilter.h"
#include "HashTable.h"
#include "Operation.h"
#include "MurmurHash.h"
#include "Value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fp1, *fp2, *fp3;
    uint8_t  uint8_IP[4] = {};
    uint32_t IP_2sin = 0, pref_len = 0, Prefix_len = 0 , result = 0 , i;
    uint32_t ALREADY = 0; //0:already exist IP-data in BloomFilter  1:not
    uint32_t int_IPaddress[DATA_SIZE], MatchVector[NAME_PREF_LEN]={};

    //unsigned char *inputFile = "Input-IP-Address.data";
    unsigned char *inputFile = "Register-Name-Prefix.data";
    unsigned char buf[EIGHT_BIT];
    unsigned char *name, *hop;
    unsigned char *ip, *pref;//, *hop;

    //unsigned char *searchFile = "search-IP-Address.data";
    unsigned char *searchFile = "Search-Name-Prefix-NextHop.data";
    unsigned char buf2[EIGHT_BIT];

    unsigned char *ip2, *hop2;
    uint32_t hash1, hash2, hash3, nisin = 0;
    uint32_t EXIST = 0; //0:exist search-IP-data in BloomFilter  1:not

    unsigned char *outputFile = "Output-NextHop.data";

    if((fp1 = fopen(inputFile, "r")) != NULL) {
      while(fgets(buf, sizeof(buf), fp1) != NULL) {
		name = strtok(buf, ",");
		hop  = strtok(NULL, "\n");
		makeFIB(LIST, name, hop, 0);
		//makeHash(LIST);
		//splitName(list,buf);
		//list_print(list);
		//splitNamePrefixAndInputBF(buf);//syori : ~~ store BF made
      }
	  //printf("OK\n");
	  makeHash(LIST);
      storeBloomFilter(LIST, BF);
    }
	//printf("OK\n");
    if(((fp2 = fopen(searchFile, "r")) != NULL) && ((fp3 = fopen(outputFile, "w")) != NULL)) {
      while(fgets(buf2, sizeof(buf2), fp2) != NULL) {
		ip2  = strtok(buf2, "\n");
	    LongestPrefixMatching(LIST, BF, ip2, MatchVector);//MatchVector作成まで	
		//printf("OK\n");
		printf("[ %s ]'s MatchVector = ",ip2);
		for(int i=NAME_PREF_LEN-1; i>=0; i--){
		  printf("%d", MatchVector[i]);
		  if((i==8)||(i==16)||(i==24)) printf(" ");
		}
		printf("\n");
		
      }
    }
	//list_print(LIST);
	//printf("OK\n");
	
	/*for(int i=0; i<=NAME_PREF_LEN; i++){
	  for(int j=0;j<TEN_BIT;j++){
	  printf("%u",BF[i][j].bit);
	  }
	  printf("\n");
	  }*/
	/*printf("MatchVector = ");
	for(int i=NAME_PREF_LEN-1; i>=0; i--){
	  printf("%d", MatchVector[i]);
	  if((i==8)||(i==16)||(i==24)) printf(" ");
	}
	printf("\n");*/
	return 0;
}
/*for(int i=0;i<=NAME_PREF_LEN;i++){
  for(int j=0;j<TEN_BIT;j++){
	if(NPBF[i][j].bit){
	  struct NameList *head = MNP[i][j].next;
	  for(; head != NULL; head = head->next) {
	    printf("NPBF[%d][%d] = %s\n",i,j,head->NamePrefix);
	  }
	  free(head);
	}
      }
      }*/
/*
    if((fp1 = fopen(inputFile, "r")) != NULL) {
        while(fgets(buf, sizeof(buf), fp1) != NULL) {
            ip   = strtok(buf, ",");
            pref = strtok(NULL, ",");
            hop  = strtok(NULL, ",");
            changeIPtoUINT(ip, uint8_IP);
            pref_len = atoi(pref);

            if((0 <= pref_len) && (pref_len <= 32)) {
                IP_2sin = uint8_IP[0] << 24 | uint8_IP[1] << 16 | uint8_IP[2] << 8 | uint8_IP[3];
                IP_2sin = IP_2sin >> (32 - pref_len);

                for(i = 1; i <= 3; i++) {
                    MurmurHash3_uint32_uint32(IP_2sin, i, (void *)&result);
                    BF[pref_len][result % TEN_BIT].bit = 1;
		    //storeBloomFilter(IP, ip, result, &ALREADY, pref_len);
                }

                if(ALREADY == 0) { //FIB don't exist pattern
                    MurmurHash3_uint32_uint32(IP_2sin, 10, (void *)&result);
                    storeHashTable(HT, ip, hop, result, IP_2sin, pref_len);
                }
            }
        }
	
        fclose(fp1);
    }

    if(((fp2 = fopen(searchFile, "r")) != NULL) && ((fp3 = fopen(outputFile, "w")) != NULL)) {
        while(fgets(buf2, sizeof(buf2), fp2) != NULL) {
            ip2  = strtok(buf2, "\n");
            changeIPtoUINT(ip2, uint8_IP);
            IP_2sin = uint8_IP[0] << 24 | uint8_IP[1] << 16 | uint8_IP[2] << 8 | uint8_IP[3];

            for(i = 32; i >= 1; i--) { //for(int i=32;i>=1;i--){
                nisin = IP_2sin >> (32 - i);
                MurmurHash3_uint32_uint32(nisin, 1, (void *)&hash1);
                MurmurHash3_uint32_uint32(nisin, 2, (void *)&hash2);
                MurmurHash3_uint32_uint32(nisin, 3, (void *)&hash3);
                checkBloomFilter(BF, hash1, hash2, hash3, &EXIST, i, MatchVector);
            }

            EXIST = 0;

            for(i = 32; (i >= 1) && (EXIST == 0); i--) { //for(int i=32;(i>=1)&&(EXIST==0);i--){
                if(MatchVector[i - 1] == 1) {
                    nisin = IP_2sin >> (32 - i);
                    MurmurHash3_uint32_uint32(nisin, 10, (void *)&result);
                    checkHashTable(HT, ip2, result, nisin, i, &EXIST, &hop2);

                    if(EXIST == 1) {
                        fprintf(fp3, "IP[%s]'s NextHop(ExM) = %s", ip2, hop2);
                        break;
                    } else if(EXIST == 2) {
                        fprintf(fp3, "IP[%s]'s NextHop(LPM) = %s", ip2, hop2);
                        break;
                    }
                }

                if((i == 1) && (EXIST == 0)) {
                    fprintf(fp3, "IP[%s]'s NextHop data not exist in Hash Table.\n", ip2);
                }
            }
        }

        fclose(fp2);
        fclose(fp3);
    }

    return 0;
}
*/
