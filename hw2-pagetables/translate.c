#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "allocate.h"
#include "translate.h"

// Constants
const int ppn_bits = ADDRESS_BITS - POBITS; // todo use #define?
// const int vpn_bits = (ADDRESS_BITS - POBITS) / LEVELS;
const int vpn_bits = POBITS - 3; // log_2(2^POBITS / 2^3) bits per VPN
const size_t all_ones = ~0;

// Translation Functions

size_t get_vpn(size_t va, int level) {
    const int bits_after = POBITS + vpn_bits * (LEVELS - level); // bits after vpn part
    
    // const int end_bit = (ADDRESS_BITS - 1) - bits_after; // end bit of vpn part
    // const int start_bit = end_bit - (vpn_bits - 1); // start bit of vpn part    
    // printf("- Start = %d, End = %d\n", start_bit, end_bit);
    
    size_t va_shifted = va >> bits_after; // Remove bits after vpn part
    const size_t bitmask = ~(all_ones << (vpn_bits)); // Remove bits before vpn part
    return va_shifted & bitmask;
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
    return ppn << POBITS; // use table size?
    // return ppn * table_size_entries; // use table size?
}