#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "allocate.h"
#include "translate.h"

// Constants
const int pn_bits = 64 - POBITS;

// Translation Functions

size_t get_page_number(size_t address) {
    return address >> POBITS;
}

size_t get_page_offset(size_t address) {
    return (address << pn_bits) >> pn_bits;
}

int is_page_valid(size_t entry) {
    return entry & 1;
}

size_t get_page_address(size_t page_num) {
    return page_num << POBITS;
}