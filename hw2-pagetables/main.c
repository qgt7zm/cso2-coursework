#include "config.h"
#include "mlpt.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void test_valid_entry(size_t entry) {
    if (is_valid(entry)) {
        printf("0x%lx is a valid entry.\n", entry);
    } else {
        printf("0x%lx is not valid entry.\n", entry);
    }
}

void test_page_number(size_t entry) {
    printf("The page number for 0x%lx is 0x%lx.\n", entry, get_page_number(entry));
}

void test() {
    test_valid_entry(0x123456789); // valid
    test_valid_entry(0x123456788); // invalid

    test_page_number(0x1234567812345679); // 0x1234567812345
    test_page_number(0x98765431009); // 0x98765431
}

int main(int argc, char *argv[]) {
    initialize();

    test();

    size_t page_table[table_size];

    printf("Program completed.\n");
    return 0;
}