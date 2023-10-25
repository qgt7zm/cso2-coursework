#include <stdlib.h>

#ifndef CACHEUTIL_H
#define CACHEUTIL_H

// Constants

#define NUM_SETS 16
#define NUM_WAYS 4

#define INDEX_BITS 4
#define TAG_BITS (64 - INDEX_BITS - POBITS)

// Structs

typedef struct {
    int valid;
    size_t tag;
    size_t ppn;
    int used_order; // How recently way was used
} tlb_entry;

typedef struct {
    tlb_entry ways[NUM_WAYS];
    int size; // Number of valid ways
} tlb_set;

// Globals

extern tlb_set tlb_table[NUM_SETS];

// Bit Helper Functions

/** Get the virtual page number of the given virtual address. */
size_t get_vpn(size_t va);

/** Get the tag of the given virtual page number. */
size_t get_tag(size_t vpn);

/** Get the index of the given virtual page number. */
size_t get_index(size_t vpn);

// Cache Getter Functions

/** Get the set in the TLB cache at with the given index. */
tlb_set *get_set(int index);

/** Get the entry in the given set at the given way. */
tlb_entry *get_entry(tlb_set *set, int way);

/** Get the least recently used way in the given set, or -1 if the set is empty. */
int get_lru_way(tlb_set *set);

// Cache Setter Functions

/** Replaces the exisitng entry at the given way in the given set. */
void replace_entry(tlb_set *set, int way, size_t tag, size_t ppn);

/** Adds a new entry to end of the given set. */
void add_entry(tlb_set *set, size_t tag, size_t ppn);

/**
 * Update the recently used order for each valid way in the set, and set the given way
 * as the most recently used.
 */
void update_used_order(tlb_set *set, int mru_way);

#endif