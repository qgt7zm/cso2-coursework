#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "allocate.h"
#include "translate.h"

// Constants
const int ppn_bits = ADDRESS_BITS - POBITS; // todo put in header?
const int vpn_bits = (ADDRESS_BITS - POBITS) / LEVELS;
const size_t all_ones = ~0;

// Translation Functions

size_t get_vpn(size_t va, int level) {
    const int start_bit = vpn_bits * (level - 1); // start bit of vpn part
    const int end_bit = start_bit + vpn_bits - 1; // end bit of vpn part
    const int bits_after_ = ADDRESS_BITS - (end_bit + 1); // bits after vpn part

    // printf("Virtual address = 0x%lx\n", va);
    // printf("- Start = %d, End = %d\n", start_bit, end_bit);
    
    size_t va_shifted = va >> bits_after_; // Remove bits after vpn part
    // printf("- After shift = 0x%0.16lx\n", va_shifted);

    // Remove bits before vpn part
    const size_t bitmask = ~(all_ones << (vpn_bits));
    // printf("- Bitmask = 0x%0.16lx\n", bitmask);

    size_t vpn = va_shifted & bitmask;
    // printf("- After bitmask = 0x%lx\n", vpn);
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