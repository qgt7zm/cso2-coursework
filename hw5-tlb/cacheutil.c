#include <stdio.h>

#include "config.h"

#include "cacheutil.h"

// Constants

const size_t all_ones = -1;

// Globals

tlb_set tlb_table[NUM_SETS];

// Bit Helper Functions

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

// Cache Getter Functions

tlb_set *get_set(int index) {
    return &(tlb_table[index]);
}

tlb_entry *get_entry(tlb_set *set, int way) {
    return &(set->ways[way]);
}

int get_lru_way(tlb_set *set) {
    for (int way = 0; way < set->size; way++) {
        tlb_entry *entry = get_entry(set, way);
        if (entry->used_order == set->size) {
            return way;
        }
    }
    return -1;
}

// Cache Setter Functions

void replace_entry(tlb_set *set, int way, size_t tag, size_t ppn) {
    tlb_entry *entry = get_entry(set, way);
    entry->tag = tag;
    entry->ppn = ppn;
    printf("Replaced way %d\n", way);
}

void add_entry(tlb_set *set, size_t tag, size_t ppn) {
    tlb_entry *entry = get_entry(set, set->size);
    set->size += 1;

    entry->valid = 1;
    entry->tag = tag;
    entry->ppn = ppn;
    printf("Added way %d\n", set->size);
}

void update_used_order(tlb_set *set, int mru_way) {
    printf("MRU way: %d\n", mru_way);
    int prev_used = get_entry(set, mru_way)->used_order; // Old value of LRU bit

    for (int way = 0; way < set->size; way++) { // Ignore invalid ways
        tlb_entry *entry = get_entry(set, way);
        printf("Updating way %d:\n", way);
        printf("- Old used order: %d\n", entry->used_order);

        if (way == mru_way) {
            // Set as most recently used
            entry->used_order = 1; 
        } else if (prev_used == 0)  {
            // If adding a way increment all used values
            entry->used_order += 1;
        } else if (entry->used_order <= prev_used) {
            // If replacing a way only increment more recently used
            entry->used_order += 1;
        }
        printf("- New used order: %d\n", entry->used_order);
    }
}