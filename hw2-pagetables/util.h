#include <stdio.h>

#ifndef UTIL_H
#define UTIL_H

/** The number of bits for the page number in an entry, equal to 64 - POBITS. */
extern int pn_bits;

/** The number of entries in a page table, equal to (2^POBITS) / 8 bytes. */
extern int table_size;

/** Sets ptbr to 0. */
void initialize();

/** The physical page number of an address, equal to the first 64 - POBITS bits. */
size_t get_page_number(size_t address);

/** The page offset of an address, equal to the last POBITS bits. */
size_t get_page_offset(size_t address);

/** The base address of a physical page number, equal to address * 2^POBITS. */
size_t get_page_address(size_t page_num);

/** Whether an entry points to valid data, according to the last bit. */
int is_valid(size_t entry);

/** Get the data stored under an entry, either a page table entry or physical address */
size_t get_data(size_t entry);

#endif