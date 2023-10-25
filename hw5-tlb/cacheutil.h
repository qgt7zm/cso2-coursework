#include <stdlib.h>

#ifndef CACHEUTIL_H
#define CACHEUTIL_H

// Constants

#define NUM_SETS 16
#define NUM_WAYS 4

#define INDEX_BITS 4
#define TAG_BITS (64 - INDEX_BITS - POBITS)

// Structs

struct tlb_entry {
    int valid;
    size_t tag;
    size_t ppn;
    int used; // how recently way was used
};

struct tlb_set {
    struct tlb_entry ways[NUM_WAYS];
};

// Functions

size_t get_vpn(size_t va);

size_t get_tag(size_t vpn);

size_t get_index(size_t vpn);

#endif