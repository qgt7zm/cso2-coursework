#include <stdio.h>

#ifndef ALLOCATE_H
#define ALLOCATE_H

// Constants

/** The size of a page table or page, equal to 2^POBITS bytes. */
#define TABLE_SIZE_BYTES 2 << (POBITS - 1)

// Allocation Functions

/** Returns the pointer to the root table (ptbr). */
size_t *get_root_table();

/** Prints out debug information and sets ptbr to 0. */
void initialize();

/** Creates the root page table and sets ptbr. */
void create_root_table();

/**
 * Initializes a table on the heap using posix_memalign, sets the memory to zero,
 * and returns the address of the table.
 */
size_t allocate_table(size_t *table);

/**
 * Allocates a page under the given virtual page number and returns the
 * physical page number.
 */
size_t create_page(size_t vpn);

#endif