#include <stdio.h>

#ifndef UTIL_H
#define UTIL_H

/** The number of bits for the page number in an entry, equal to 64 - POBITS. */
extern const int pn_bits;

/** The number of bits in a page table, equal to 2^POBITS bits. */
extern const int table_bits;

/** The number of entries in a page table, equal to (2^POBITS) / 8 bytes. */
extern const int table_size;

extern size_t* root_table;

/** Sets ptbr to 0. */
void initialize();

/** Sets ptbr as a pointer to the root array. */
void set_ptbr(size_t root_table[]);

/** Create the root page table and set ptbr. */
void create_table();

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