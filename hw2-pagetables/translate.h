#include <stdio.h>

#ifndef TRANSLATE_H
#define TRANSLATE_H

/** The number of bits for the page number in an entry, equal to 64 - POBITS. */
extern const int pn_bits;

/** 
 * Get the virtual page number of an address or entry, equal to the first
 * 64 - POBITS bits.
 */
size_t get_page_number(size_t address);

/**
 * Get the page offset of a virtual or physical address, equal to the last
 * POBITS bits.
 */
size_t get_page_offset(size_t address);

/** 
 * Whether an page table entry points to valid page or table, according to
 * the last bit in an entry.
 */
int is_valid(size_t entry);

/** 
 * Get the base address of a physical page number, equal to page_num * 2^POBITS.
 */
size_t get_page_address(size_t page_num);

#endif