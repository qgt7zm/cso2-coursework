#include "config.h"
#include "mlpt.h"

#include <stdio.h>
#include <stdlib.h>

size_t ptbr;
int pn_bits = 64 - POBITS;

void initialize() {
    ptbr = 0;
    printf("Set ptbr to 0.\n");
}

/** Whether an entry points to a valid page table entry or address. */
int is_valid(size_t entry) {
    return entry & 1; // Check last bit
}

/** The physical page number of a valid entry, taken from the first 64-POBITS bits. */
size_t get_page_number(size_t entry) {
    return entry >> POBITS;
}

void test_valid_entry(size_t entry) {
    if (is_valid(entry)) {
        printf("0x%lx is a valid entry.\n", entry);
    } else {
        printf("0x%lx is not valid entry.\n", entry);
    }
}

void test_page_number(size_t entry) {
    printf("The page number of 0x%lx is 0x%lx.\n", entry, get_page_number(entry));
}

void test() {
    test_valid_entry(0x123456789); // valid
    test_valid_entry(0x123456788); // invalid

    test_page_number(0x1234567812345679); // 0x1234567812345
}

int main(int argc, char *argv[]) {
    ptbr = 0;
    initialize();

    test();

    printf("Program completed.\n");
    return 0;
}