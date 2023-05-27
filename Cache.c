//
// cachelab.c - Cache lab template
//
//

#include <stdio.h>
#include "cachelab.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


// more libraries if needed for your program

// print result of cache simulation showing hit number, miss number, miss rate, and total running time
void printResult(int hits, int misses, int missRate, int runTime) {
    printf("[result] hits: %d misses: %d miss rate: %d%% total running time: %d cycle\n",hits, misses, missRate, runTime);
}

// main function should be coded here

// Structure of a cache line
typedef struct {
    int valid;
    unsigned long tag;
    int setIndex;
    // LRU Features 
    int lastUsedCounter;
    int lru;
} cacheLine;

// Structure of a set
typedef struct {
    cacheLine *lines;
}set;

// Make the bitwise mask   
unsigned long makeMask(int bits) {
    unsigned long mask = 0;
    for (int i = 0; i < bits; i++) {
        mask = (mask << 1) + 1;
    }
    return mask;
}

// Get the tag from the address
unsigned long getTag(unsigned long address, int s, int b) {
    return address >> (s + b);
}

int main( int argc, char *argv[] ){

    // declare variables
    int m = -1;
    int s = -1;
    int e = 1;
    int b = -1;
    char *tracefile = NULL;
    char *replacement = NULL;
    int i;
    
    // parse input
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0) {
            m = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            s = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-e") == 0) {
            e = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-b") == 0) {
            b = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-i") == 0) {
            tracefile = argv[i+1];
        }
        else if (strcmp(argv[i], "-r") == 0) {
            replacement = argv[i+1];
        }
    }

    e = 1; // Because only using LRU

    // test inputs are being parsed right
    // printf("m: %d s: %d e: %d b: %d tracefile: %s replacement: %s\n", m, s, e, b, tracefile, replacement);

    if ( (m == -1) || (s == -1) || (e == -1) || (b == -1) || (tracefile == NULL) || (replacement == NULL) ) {
        printf("Error: missing arguments please try again\n");
        return 0;
    }

    // open trace file
    FILE *fp = fopen(tracefile, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", tracefile);
        return 0;
    }

    // declare variables

    char operation;
    unsigned long address;
    int size;
    int hits = 0;
    int misses = 0;
    int runTime = 0;
    char addressString[100];

    // allocate memory for cache
    set *cache = malloc(sizeof(set) * (1 << s));
    for (int i = 0; i < (1 << s); i++) {
        cache[i].lines = malloc(sizeof(cacheLine) * e);
    }

    // initialize cache
    for (int i = 0; i < (1 << s); i++) {
        for (int j = 0; j < e; j++) {
            char *data = malloc(sizeof(char) * (1 << b));
            int mask = makeMask(b);
            cache[i].lines[j].valid = 0;
            cache[i].lines[j].tag = 0;
            cache[i].lines[j].setIndex = 0;
            cache[i].lines[j].lastUsedCounter = 0;
            cache[i].lines[j].lru = 0;
        }
    }
    

    // read the address file parse the address 
    while (fscanf(fp, "%s", addressString) == 1 ) {

        address = strtol(addressString, NULL, 16);

        

        // calculate tag, set index, and block offset
        unsigned long tag = address >> (s + b);
        unsigned long setIndex = (address >> b) & makeMask(s);
        // block offset using bitwise mask and shift
        unsigned long blockOffset = address & makeMask(b);
        // tag bits using bitwise mask and shift   
        // calculate tag bits using bitwise shift and modulo
        unsigned long tagBits = (address >> (s + b)) % (1 << m);


        /*
        // allocate memeory for cache ACCIDENTLY REINITIALIZED CACHE EVERY TIME NEW ADDRESS WAS USED
        set *cache = malloc(sizeof(set) * (1 << s));
        for (int i = 0; i < (1 << s); i++) {
            cache[i].lines = malloc(sizeof(cacheLine) * e);
        }

        // initialize cache
        for (int i = 0; i < (1 << s); i++) {
            for (int j = 0; j < e; j++) {
                char *data = malloc(sizeof(char) * (1 << b));
                int mask = makeMask(b);
                setIndex = (address >> b) & mask;
                cache[i].lines[j].valid = 0;
                cache[i].lines[j].tag = 0;
                cache[i].lines[j].setIndex = 0;
                cache[i].lines[j].lastUsedCounter = 0;
                cache[i].lines[j].lru = 0;
            }
        }

        */

        // check if hit or miss
        int hit = 0;
        int lineToReplace = -1;

        // search for invalid line or line to replace based on replacement policy
        for (int i = 0; i < e; i++) {
            if (cache[setIndex].lines[i].valid == 0) {
                // found an invalid line, use it to add the new address
                lineToReplace = i;
                break;
            } else if (cache[setIndex].lines[i].tag == tag) {
                // found a hit
                hit = 1;
                break;
            } else if (lineToReplace == -1 || cache[setIndex].lines[i].lru > cache[setIndex].lines[lineToReplace].lru) {
                // found a line to replace based on replacement policy (LRU in this case)
                lineToReplace = i;
            }
        }

        if (hit) {
            hits++;

            printf("%s H \n", addressString);

            // update LRU
            cache[setIndex].lines[i].lastUsedCounter = 0;
            for (int j = 0; j < e; j++) {
                if (cache[setIndex].lines[j].lastUsedCounter != -1) {
                    cache[setIndex].lines[j].lastUsedCounter++;
                }
            }
        } else {
            misses++;

            printf("%s M \n", addressString);

            // add address to cache
            cache[setIndex].lines[lineToReplace].tag = tag;
            cache[setIndex].lines[lineToReplace].setIndex = setIndex;
            cache[setIndex].lines[lineToReplace].valid = 1;

            /*
            // print cache
            for (int i = 0; i < (1 << s); i++) {
                for (int j = 0; j < e; j++) {
                    printf("valid: %d tag: %ld setIndex: %d lastUsedCounter: %d lru: %d\n", cache[i].lines[j].valid, cache[i].lines[j].tag, cache[i].lines[j].setIndex, cache[i].lines[j].lastUsedCounter, cache[i].lines[j].lru);
                }
            }
            */

            // update LRU
            cache[setIndex].lines[lineToReplace].lastUsedCounter = 0;
            for (int j = 0; j < e; j++) {
                if (cache[setIndex].lines[j].lastUsedCounter != -1) {
                    cache[setIndex].lines[j].lastUsedCounter++;
                }
            }
        }


        /* OLD CODE
        // check if hit or miss
        int hit = 0;
        for (int i = 0; i < s; i++) {
            if (cache[setIndex].lines[i].valid == 1 && cache[setIndex].lines[i].tag == tag) {
                
                hit = 1;
                hits++;

                // add address to cache
                cache[setIndex].lines[i].tag = tag;
                cache[setIndex].lines[i].setIndex = setIndex;
                cache[setIndex].lines[i].valid = 1;

                // update LRU
                cache[setIndex].lines[i].lastUsedCounter = 0;
                for (int j = 0; j < e; j++) {
                    if (cache[setIndex].lines[j].lastUsedCounter != -1) {
                        cache[setIndex].lines[j].lastUsedCounter++;
                    }
                }
                break;

            } else {
                misses++;

                // add address to cache
                cache[setIndex].lines[i].tag = tag;
                cache[setIndex].lines[i].setIndex = setIndex;
                cache[setIndex].lines[i].valid = 1;
                // print cache
                for (int i = 0; i < (1 << s); i++) {
                    for (int j = 0; j < e; j++) {
                        printf("valid: %d tag: %ld setIndex: %d lastUsedCounter: %d lru: %d\n", cache[i].lines[j].valid, cache[i].lines[j].tag, cache[i].lines[j].setIndex, cache[i].lines[j].lastUsedCounter, cache[i].lines[j].lru);
                    }
                }

                // update LRU
                cache[setIndex].lines[i].lastUsedCounter = 0;
                for (int j = 0; j < e; j++) {
                    if (cache[setIndex].lines[j].lastUsedCounter != -1) {
                        cache[setIndex].lines[j].lastUsedCounter++;
                    }
                }
                break;
            }
        }
        */

    }
        int hitTime = 1;
        int missPenalty = 100;
        int totalAccesses = hits + misses;
        double missRate = ((double) misses / (double) totalAccesses) * 100; 

        //printf("totalAccess: %d", totalAccesses);
        //printf("miss rate: %f", missRate);

        double accessTime = 1 + (missRate) * missPenalty;
        runTime = totalAccesses * ( missRate + 1) ;

        // print result
        printResult(hits, misses, missRate, runTime);

   

    // close trace file
    fclose(fp);

    return 0;
}


// GOOD LUCK!

// A simple main method to test your program

