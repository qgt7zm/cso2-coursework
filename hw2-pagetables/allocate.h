#include <stdio.h>

#ifndef ALLOCATE_H
#define ALLOCATE_H

// Constants

/** The number of bytes in a page table, equal to (2^POBITS) bytes. */
extern const int table_size_bytes;

/** The number of entries in a page table, equal to 2^POBITS / 8. */
extern const int table_size_entries;

// Allocation Functions

/** Prints out debug information and sets ptbr to 0. */
void initialize();

/** Creates the root page table, sets ptbr, and returns ptbr. */
size_t *create_root_table();

/**
 * Allocates a page under the given virtual page number and returns the
 * physical page number.
 */
size_t create_page(size_t vpn);

#endif