#include "BloomFilter.h"
//#include "HashTable.h"
#include "Operation.h"
#include "MurmurHash.h"
#include "Value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fp1, *fp2, *fp3;
    uint32_t MatchVector[NAME_PREF_LEN]={};
	
    unsigned char *inputFile = "Register-Name-Prefix.data";
    unsigned char buf[EIGHT_BIT];
    unsigned char *name, *interface;
    unsigned char *ip, *pref;//, *hop;
	uint32_t port = 0;

    unsigned char *searchFile = "Search-Name-Prefix-NextHop.data";
    unsigned char *outputFile = "Interface.data";
    unsigned char buf2[EIGHT_BIT];	
    unsigned char *search;
	uint32_t next, DEFAULT = 0;
	
    //unsigned char *outputFile = "Output-NextHop.data";

    if((fp1 = fopen(inputFile, "r")) != NULL) {
      while(fgets(buf, sizeof(buf), fp1) != NULL) {
		name = strtok(buf, ",");
		interface = strtok(NULL, "\n");
		
		makeInterface(MBF, name, interface, &port);//make Merged BloomFilter
		makeFIB(LIST, name, interface, &port);//make stage1 BloomFilter
		port = 0;
      }
	  makeHash(LIST);
      storeStage1BloomFilter(LIST, BF);
	  
    }
	//list_print(LIST);
	//Stage1_BloomFilter_print(BF);
	//Merged_BloomFilter_print(MBF);
	//return 0;
    if(((fp2 = fopen(searchFile, "r")) != NULL) && ((fp3 = fopen(outputFile, "w")) != NULL)) {
      while(fgets(buf2, sizeof(buf2), fp2) != NULL) {
		search = strtok(buf2, "\n");
	    LongestPrefixMatching(LIST, BF, search, MatchVector);//MatchVector作成まで
	    //MatchVector_print(ip2, MatchVector);
		checkInterface(MBF, search, MatchVector, &DEFAULT, &next);//Next Port Number Outputまで
		if(DEFAULT) 
		  fprintf(fp3,"NamePrefix[%s]'s NextInterface = %u\n",search, next);
		else
		  fprintf(fp3,"NamePrefix[%s]'s NextInterface = default\n",search);		
		next = 0;
		DEFAULT = 0;
      }
    }
	
	return 0;
}
  
