#include <stdio.h>

#ifndef ALLOCATE_H
#define ALLOCATE_H

// Constants

/** The number of bits in a page table, equal to 2^POBITS bits. */
extern const int table_size_bits;

/** The number of entries in a page table, equal to (2^POBITS) / 8 bytes. */
extern const int table_size_bytes;

// Globals

/** The pointer to the root table (ptbr). */
extern size_t* root_table;

// Allocation Functions

/** Sets ptbr to 0. */
void initialize();

/** Create the root page table and set ptbr. */
void create_table();

/** Sets ptbr as a pointer to the root array. */
void set_ptbr(size_t root_table[]);

#endif