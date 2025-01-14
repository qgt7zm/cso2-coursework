#include <stdio.h>

#ifndef TRANSLATE_H
#define TRANSLATE_H

/** The number of bits in an address, equal to 64. */
#define ADDRESS_BITS 64

// Translation Functions

/** 
 * Get the virtual page number part of a virtual address for the given level, 
 * equal to VPN_BITS bits starting from VPN_BITS * (level - 1).
 */
size_t get_vpn(size_t va, int level);

/**
 * Get the page offset of a virtual or physical address, equal to the last
 * POBITS bits.
 */
size_t get_page_offset(size_t address);

/** 
 * Whether an page table entry points to valid page or table, according to
 * the last bit in an entry.
 */
int is_page_valid(size_t entry);

/** 
 * Get the physical page number from a page table entry, equal to the first
 * 64 - POBITS bits.
 */
size_t get_ppn(size_t entry);

/** 
 * Get the base address of a physical page number, equal to PPn * 2^POBITS.
 */
size_t get_page_address(size_t ppn);

#endif