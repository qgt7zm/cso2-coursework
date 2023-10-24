#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "mlpt.h"
#include "tlb.h"

#include "cacheutil.h"

struct tlb_entry tlb_table[NUM_SETS];

void tlb_clear() {
    memset(tlb_table, 0, NUM_SETS);
}

int tlb_peek(size_t va) {
    return 0;
}

void update_entry(struct tlb_entry *entry, size_t tag, size_t ppn) {
    entry->tag = tag;
    entry->ppn = ppn;
}

size_t tlb_translate(size_t va) {
    printf("Looking up VA 0x%lx\n", va);

    size_t vpn = get_vpn(va);
    size_t index = get_index(vpn);
    size_t tag = get_tag(vpn);

    printf("- Index: 0x%lx\n", index);
    printf("- Tag: 0x%lx\n", tag);

    struct tlb_entry *entry = &tlb_table[index];
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

    // TODO update recently used;
    return ppn;
}