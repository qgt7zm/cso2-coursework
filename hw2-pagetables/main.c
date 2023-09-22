#include "config.h"
#include "mlpt.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void test_translate_single(size_t page_table[], size_t address) {
    printf("Testing virtual address 0x%lx:\n", address);

    size_t vpn = get_page_number(address);
    printf("- Virtual page number = 0x%lx\n", vpn);

    size_t page_off = get_page_offset(address);
    printf("- Page offset = 0x%lx\n", page_off);

    size_t pte = page_table[vpn];
    printf("- Page entry = 0x%lx\n", pte);

    if (is_valid(pte)) {
        printf("- Entry is valid\n");
    } else {
        printf("- Entry is invalid\n");
        printf("\n");
        return;
    }

    size_t ppn = get_page_number(pte);
    printf("- Physical page number = 0x%lx\n", ppn);

    size_t page_add = get_page_address(ppn);
    printf("- Page address = ptbr + 0x%lx\n", page_add);

    size_t p_add = page_add + page_off;
    printf("- Physical address = ptbr + 0x%lx\n", p_add);

    printf("\n");
}

void test(size_t page_table[]) {
    page_table[0x123] = 0xabc001;
    page_table[0x124] = 0xdef000;

    printf("\n");
    test_translate_single(page_table, 0x123456); // Page 0x123, 0xabc456
    test_translate_single(page_table, 0x124789); // Page 0x124, invalid
}

int main(int argc, char *argv[]) {
    initialize();

    size_t page_table[table_size]; // stores PTEs
    test(page_table);

    printf("Program completed.\n");
    return 0;
}