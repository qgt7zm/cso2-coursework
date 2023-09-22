#include <stdio.h>

#ifndef UTIL_H
#define UTIL_H

/** The number of pits for the page number in an entry, equal to 64 - POBITS. */
extern int pn_bits;

/** The number of entries in a page table, equal to (2^POBITS) / 8 bytes. */
extern int table_size;

/** Sets ptbr to 0. */
void initialize();

/** Whether an entry points to a valid page table entry or address. */
int is_valid(size_t entry);

/** The physical page number of a valid entry. */
size_t get_page_number(size_t entry);

#endif