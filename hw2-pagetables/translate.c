#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "allocate.h"
#include "translate.h"

// Constants
const int ppn_bits = ADDRESS_BITS - POBITS;
const int vpn_bits = (ADDRESS_BITS - POBITS) / LEVELS;
const size_t all_ones = ~0;

// Translation Functions

size_t get_vpn(size_t va, int level) {
    const int startBit = vpn_bits * (level - 1); // start bit of vpn part
    const int endBit = startBit + vpn_bits; // end bit of vpn part
    const int bitsAfter = ADDRESS_BITS - endBit; // bits after end bit

    printf("Virtual address = 0x%lx\n", va);
    printf("- Start = %d, End = %d\n", startBit, endBit);

    // Select bits from startBit to endBit
    const size_t bitmask = (all_ones >> (startBit + POBITS)) << bitsAfter;
    printf("- Bitmask = 0x%lx\n", bitmask);

    size_t vpn = va & bitmask;
    printf("- After bitmask = 0x%lx\n", vpn);
    
    printf("- Shift right = %d bits\n", bitsAfter);
    vpn >>= bitsAfter;
    return vpn;
}

size_t get_page_offset(size_t address) {
    return (address << ppn_bits) >> ppn_bits;
}

int is_page_valid(size_t entry) {
    return entry & 1;
}

size_t get_ppn(size_t pa) {
    return pa >> POBITS;
}

size_t get_page_address(size_t ppn) {
    return ppn << POBITS;
}