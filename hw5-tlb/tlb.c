#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "mlpt.h"
#include "tlb.h"

#include "cacheutil.h"

tlb_set tlb_table[NUM_SETS];

// Helper Functions

/**
 * Get the set in the TLB cache at with the given index.
 */
tlb_set *get_set(int index) {
    return &(tlb_table[index]);
}

/**
 * Get the entry in the given set at the given way.
 */
tlb_entry *get_entry(tlb_set *set, int way) {
    return &(set->ways[way]);
}

/**
 * Get the least recently used way in the given set, or -1 if the set is empty.
 */
int get_lru_way(tlb_set *set) {
    for (int way = 0; way < set->size; way++) {
        tlb_entry *entry = get_entry(set, way);
        if (entry->used == set->size) {
            return way;
        }
    }
    return -1;
}

/**
 * Replaces the exisitng entry at the given way in the given set.
 */
void replace_entry(tlb_set *set, int way, size_t tag, size_t ppn) {
    tlb_entry *entry = get_entry(set, way);
    entry->tag = tag;
    entry->ppn = ppn;
    printf("Replaced way %d\n", way);
}

/**
 * Adds a new entry to the given set.
 */
void add_entry(tlb_set *set, size_t tag, size_t ppn) {
    tlb_entry *entry = get_entry(set, set->size);
    set->size += 1;

    entry->valid = 1;
    entry->tag = tag;
    entry->ppn = ppn;
    printf("Added way %d\n", set->size);
}

/**
 * Update the recently used order for each valid way in the set, and set the given way
 * as the most recently used.
 */
void update_used_order(tlb_set *set, int mru_way) {
    printf("MRU way: %d\n", mru_way);
    int prev_used = get_entry(set, mru_way)->used; // Old value of LRU bit

    for (int way = 0; way < set->size; way++) { // Ignore invalid ways
        tlb_entry *entry = get_entry(set, way);
        printf("Updating way %d:\n", way);
        printf("- Old used order: %d\n", entry->used);

        if (way == mru_way) {
            // Set as most recently used
            entry->used = 1; 
        } else if (prev_used == 0)  {
            // If adding a way increment all used values
            entry->used += 1;
        } else if (entry->used <= prev_used) {
            // If replacing a way only increment more recently used
            entry->used += 1;
        }
        printf("- New used order: %d\n", entry->used);
    }
}

// TLB Functions

void tlb_clear() {
    memset(tlb_table, 0, NUM_SETS);
}

/**
 * Get the way in the set that contains the given tag, or -1 if the tag is not present.
 */
int get_way(tlb_set *set, size_t tag) {
    for (int way = 0; way < NUM_WAYS; way++) {
        tlb_entry *entry = get_entry(set, way);

        printf("Way %d:\n", way);
        printf("- Valid: %d\n", entry->valid);
        printf("- Tag: 0x%lx\n", entry->tag);
        printf("- Used: %d\n", entry->used);

        if (!entry->valid) {
            printf("- Set does not contain tag\n");
            return -1;
        } else if (entry->tag == tag) {
            // Entry has the same tag
            printf("- Set contains tag\n");
            return way;
        }
    }

    // Entry has wrong tag
    printf("- Set does not contain tag\n");
    return -1;
}

int tlb_peek(size_t va) {
    printf("Peeking at VA 0x%lx\n", va);

    // Get set at index for VA
    size_t vpn = get_vpn(va);
    size_t index = get_index(vpn);
    size_t tag = get_tag(vpn);

    printf("- Index: 0x%lx\n", index);
    printf("- Tag: 0x%lx\n", tag);

    // Look up the VPN in the cache
    tlb_set *set = get_set(index);
    int way = get_way(set, tag);

    if (way == -1) {
        return 0;
    } else {
       tlb_entry *entry = get_entry(set, way);
       printf("- Last used: %d\n", entry->used);
       return entry->used;
    }
}

size_t tlb_translate(size_t va) {
    printf("Looking up VA 0x%lx\n", va);

    // Get set at index for VA
    size_t vpn = get_vpn(va);
    size_t index = get_index(vpn);
    size_t tag = get_tag(vpn);

    printf("- Index: 0x%lx\n", index);
    printf("- Tag: 0x%lx\n", tag);

    // Look up the VPN in the cache
    tlb_set *set = get_set(index);
    int way = get_way(set, tag);

    size_t ppn;
    if (way == -1) {
        // VPN is not present
        ppn = translate(va);

        if (set->size < NUM_WAYS) {
            // Add a new way if set is not full
            add_entry(set, tag, ppn);
            way = set->size;
        } else {
            // Replace LRU way if set is full
            int way = get_lru_way(set);
            replace_entry(set, way, tag, ppn);
        }

        // tlb_entry *new_entry = get_entry(set, way);
        // new_entry->valid = 1;
        // new_entry->tag = tag;
        // new_entry->ppn = ppn;
    } else {
        // VPN is present
        tlb_entry *entry = get_entry(set, way);
        printf("- Last used: %d\n", entry->used);
        ppn = entry->ppn;
    }

    update_used_order(set, way);
    return ppn;
}