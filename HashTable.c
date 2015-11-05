#include "HashTable.h"
#include <stdlib.h>

void storeHashTable(struct HashTable(*ht)[TEN_BIT], unsigned char *key, unsigned char *hop, uint32_t hash, uint32_t ip_2sin, uint32_t prefix)
{
    uint32_t num = hash % TEN_BIT;
    struct entry *node, *head;
    int Break = 0;

    if(ht[prefix][num].next == NULL) { //Case: not hashing
        if((node = (struct entry *)malloc(sizeof(struct entry))) == NULL) {
            printf("malloc error.\n");
            exit(1);
        }

        strcpy(node->IPaddress, key);
        strcpy(node->NextHop, hop);
        node->nisin = ip_2sin;
        ht[prefix][num].next = node;
        node->next = NULL;
    } else {
        head = ht[prefix][num].next;

        for(; head != NULL; head = head->next) {
            if((strcmp(head->IPaddress, key)) == 0) {
                Break++;
                break;
            }
        }

        if(Break == 0) {
            if((node = (struct entry *)malloc(sizeof(struct entry))) == NULL) {
                printf("malloc error.\n");
                exit(1);
            }

            strcpy(node->IPaddress, key);
            strcpy(node->NextHop, hop);
            node->nisin = ip_2sin;
            head = node;
            node->next = NULL;
        }
    }
}


void checkHashTable(struct HashTable(*ht)[TEN_BIT], unsigned char *key, uint32_t hash, uint32_t ip_2sin, uint32_t prefix, uint32_t *comp, unsigned char **hop)
{
    uint32_t num = hash % TEN_BIT;
    struct entry *name = ht[prefix][num].next;

    //printf("%u\n",ip_2sin);
    for(; name != NULL; name = name->next) {
        if((strcmp(name->IPaddress, key)) == 0) {
            *hop = name->NextHop;
            *comp = 1;
            break;
        } else if(name->nisin == ip_2sin) {
            *hop = name->NextHop;
            *comp = 2;
            break;
        }
    }
}
