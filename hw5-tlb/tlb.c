#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "mlpt.h"
#include "tlb.h"

#include "cacheutil.h"

struct tlb_set tlb_table[NUM_SETS];

// Helper Functions

struct tlb_set *get_set(int index) {
    // Get least recently used entry
    return &(tlb_table[index]);
}

struct tlb_entry *get_entry(int index) {
    // Get least recently used entry
    return &(tlb_table[index].ways[0]);
}

void update_entry(struct tlb_entry *entry, size_t tag, size_t ppn) {
    entry->tag = tag;
    entry->ppn = ppn;
}

void update_lru_ways(int index, int way) {
    struct tlb_entry *ways = get_set(index)->ways;
    int prev_used = ways[way].used; // Old value of LRU bit

    for (int i = 0; i < NUM_WAYS; i++) {
        struct tlb_entry *entry = &ways[i];
        if (entry->valid == 0)
            continue; // Ignore invalid ways
        else if (i == way)
            entry->used = 1; // Set the most recently used way
        else if (entry->used < prev_used)
            entry->used += 1; // Increase other ways
    }
    // Set to first
}

// TLB Functions

void tlb_clear() {
    memset(tlb_table, 0, NUM_SETS);
}

int tlb_peek(size_t va) {
    printf("Peeking at VA 0x%lx\n", va);

    size_t vpn = get_vpn(va);
    size_t index = get_index(vpn);
    size_t tag = get_tag(vpn);

    printf("- Index: 0x%lx\n", index);
    printf("- Tag: 0x%lx\n", tag);

    struct tlb_entry *entry = get_entry(index);
    printf("Entry:\n");
    printf("- Valid: %d\n", entry->valid);
    printf("- Tag: 0x%lx\n", entry->tag);

    if (entry->valid && entry->tag == tag) {
        printf("- Cache entry present\n");
        printf("- Last used: %d\n", entry->used);
        return entry->used;
    } else {
        printf("- Cache entry not present\n");
        return 0;
    }
}

size_t tlb_translate(size_t va) {
    printf("Looking up VA 0x%lx\n", va);

    size_t vpn = get_vpn(va);
    size_t index = get_index(vpn);
    size_t tag = get_tag(vpn);

    printf("- Index: 0x%lx\n", index);
    printf("- Tag: 0x%lx\n", tag);

    struct tlb_entry *entry = get_entry(index);
    printf("Entry:\n");
    printf("- Valid: %d\n", entry->valid);
    printf("- Tag: 0x%lx\n", entry->tag);

    size_t ppn;
    int fill_in_entry = 0;

    // Look up the VPN in the cache
    if (!entry->valid) {
        // Virtual address is not in cache
        printf("- Cache entry invalid\n");

        fill_in_entry = 1;
        entry->valid = 1;
    } else if (entry->tag != tag) {
        // Entry does not have same tag
        printf("- Cache entry has wrong tag\n");
        fill_in_entry = 1;
    } else {
        // Entry has the same tag
        printf("- Cache entry present\n");
        ppn = entry->ppn;
    }

    if (fill_in_entry) {
        ppn = translate(va);
        printf("- Updated entry\n");
        update_entry(entry, tag, ppn);
    }

    // TODO update recently used
    update_lru_ways(index, 0);
    return ppn;
}