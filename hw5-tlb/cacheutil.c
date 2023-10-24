#include <stdio.h>

#include "config.h"

#include "cacheutil.h"

#define NUM_SETS 16
#define NUM_WAYS 4

#define INDEX_BITS 4
#define TAG_BITS (64 - INDEX_BITS - POBITS)

const size_t all_ones = -1;

size_t get_vpn(size_t va) {
    return va >> POBITS; // Remove offset bits
}

size_t get_tag(size_t vpn) {    
    return vpn >> INDEX_BITS; // Remove index bits
}

size_t get_index(size_t vpn) {    
    const size_t bitmask = ~(all_ones << INDEX_BITS); // Remove tag bits
    // printf("Bitmask: 0x%lx\n", bitmask);
    return vpn & bitmask;
}