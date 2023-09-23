#include "config.h"
#include "mlpt.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

const size_t invalid_address = ~0;
size_t* root_table = 0;

size_t translate(size_t address) {
    printf("Translating virtual address 0x%lx:\n", address);

    if (!root_table) {
        printf("- Root page table not created yet. \n");
        printf("- Physical address = 0x%lx\n", invalid_address);
        printf("\n");
        return invalid_address;
    }

    size_t vpn = get_page_number(address);
    printf("- Virtual page number = 0x%lx\n", vpn);

    size_t page_off = get_page_offset(address);
    printf("- Page offset = 0x%lx\n", page_off);

    size_t pte = root_table[vpn];
    printf("- Page entry = 0x%lx\n", pte);

    if (is_valid(pte)) {
        printf("- Entry is valid\n");
    } else {
        printf("- Entry is invalid\n");
        printf("- Physical address = 0x%lx\n", invalid_address);
        printf("\n");
        return invalid_address;
    }

    size_t ppn = get_page_number(pte);
    printf("- Physical page number = 0x%lx\n", ppn);

    size_t page_add = get_page_address(ppn);
    printf("- Page address = 0x%lx\n", page_add);

    size_t p_add_rel = page_add + page_off;
    printf("- Physical address = 0x%lx\n", p_add_rel);
    
    size_t p_add = p_add + ptbr;
    printf("- Physical address + ptbr = 0x%lx\n", p_add);

    printf("\n");
    return p_add;
}

void test() {
    if (root_table) {
        root_table[0x123] = 0xabc001;
        root_table[0x124] = 0xdef000;
        root_table[0x125] = 0xabd001;
    }

    printf("\n");
    translate(0x123456); // Page 0x123, 0xabc456
    translate(0x124789); // Page 0x124, invalid
    translate(0x125456); // Page 0x125, 0xabd456
}

int main(int argc, char *argv[]) {
    initialize();

    create_table();
    root_table = &ptbr;

    test();

    printf("Program completed.\n");
    return 0;
}