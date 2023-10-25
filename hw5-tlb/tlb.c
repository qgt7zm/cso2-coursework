#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "mlpt.h"
#include "tlb.h"

#include "cacheutil.h"

// TLB Functions

void tlb_clear() {
    memset(tlb_table, 0, NUM_SETS * sizeof(tlb_set));
    printf("Cleared TLB cache\n");
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
        printf("- Used: %d\n", entry->used_order);

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
    const size_t vpn = get_page_number(va);
    const size_t index = get_index(vpn);
    const size_t tag = get_tag(vpn);

    printf("- Index: 0x%lx\n", index);
    printf("- Tag: 0x%lx\n", tag);

    // Look up the VPN in the cache
    tlb_set *set = get_set(index);
    int way = get_way(set, tag);

    if (way == -1) {
        return 0;
    } else {
       tlb_entry *entry = get_entry(set, way);
       printf("- Last used: %d\n", entry->used_order);
       return entry->used_order;
    }
}

size_t tlb_translate(size_t va) {
    printf("Looking up VA 0x%lx\n", va);

    // Get set at index for VA
    const size_t vpn = get_page_number(va);
    const size_t index = get_index(vpn);
    const size_t tag = get_tag(vpn);

    printf("- Index: 0x%lx\n", index);
    printf("- Tag: 0x%lx\n", tag);

    // Look up the VPN in the cache
    tlb_set *set = get_set(index);
    int way = get_way(set, tag);

    size_t ppn;
    if (way == -1) {
        // Look up if VPN is not present
        const size_t va_aligned = get_page_address(get_page_number(va));
        const size_t pa = translate(va_aligned);
        if (pa == -1) {
            return -1; // Don't update if VA is invalid
        }

        // Align address with page
        ppn = get_page_number(pa);    

        if (set->size < NUM_WAYS) {
            // Add a new way if set is not full
            add_entry(set, tag, ppn);
            way = set->size;
        } else {
            // Replace LRU way if set is full
            way = get_lru_way(set);
            replace_entry(set, way, tag, ppn);
        }
    } else {
        // Use cache if VPN is present
        const tlb_entry *entry = get_entry(set, way);
        printf("- Last used: %d\n", entry->used_order);
        ppn = entry->ppn;
    }

    update_used_order(set, way);
    const size_t pa = get_page_address(ppn) + get_page_offset(va);
    printf("PA = 0x%lx\n", pa);
    return pa;
}