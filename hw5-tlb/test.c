#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include "config.h"
#include "mlpt.h"
#include "tlb.h"

#include "cacheutil.h"

void test_split_va(size_t va) {
    printf("VA = 0x%lx\n", va);

    size_t vpn = get_vpn(va);
    printf("VPN = 0x%lx\n", vpn);

    size_t tag = get_tag(vpn);
    printf("Tag = 0x%lx\n", tag);

    size_t index = get_index(vpn);
    printf("Index = 0x%lx\n", index);
}

void example1() {
    puts("Example 1:");

    tlb_clear();
    // assert(tlb_peek(0) == 0);
    assert(tlb_translate(0) == 0x0020000);
    // assert(tlb_peek(0) == 1);
    assert(tlb_translate(0x200) == 0x20200);
    assert(tlb_translate(0x400) == 0x20400);
    // assert(tlb_peek(0) == 1);
    // assert(tlb_peek(0x200) == 1);
    assert(tlb_translate(0x2001200) == 0x2101200);
    assert(tlb_translate(0x0005200) == 0x0025200);
    assert(tlb_translate(0x0008200) == 0x0028200);
    assert(tlb_translate(0x0002200) == 0x0022200);
    // assert(tlb_peek(0x2001000) == 1);
    // assert(tlb_peek(0x0001000) == 0);
    // assert(tlb_peek(0x0004000) == 0);
    // assert(tlb_peek(0x0005000) == 1);
    // assert(tlb_peek(0x0008000) == 1);
    // assert(tlb_peek(0x0002000) == 1);
    // assert(tlb_peek(0x0000000) == 1);
    tlb_clear();
    // assert(tlb_peek(0x2001000) == 0);
    // assert(tlb_peek(0x0005000) == 0);
    // assert(tlb_peek(0x0008000) == 0);
    // assert(tlb_peek(0x0002000) == 0);
    // assert(tlb_peek(0x0000000) == 0);
    assert(tlb_translate(0) == 0x20000);
    // assert(tlb_peek(0) == 1);
    puts("");
}

void example2() {
    puts("Example 2:");
    tlb_clear();
    assert(tlb_translate(0x0001200) == 0x0021200);
    assert(tlb_translate(0x2101200) == 0x2201200);
    assert(tlb_translate(0x0801200) == 0x0821200);
    assert(tlb_translate(0x2301200) == 0x2401200);
    assert(tlb_translate(0x0501200) == 0x0521200);
    assert(tlb_translate(0x0A01200) == 0x0A21200);
    // assert(tlb_peek(0x0001200) == 0);
    // assert(tlb_peek(0x2101200) == 0);
    // assert(tlb_peek(0x2301200) == 3);
    // assert(tlb_peek(0x0501200) == 2);
    // assert(tlb_peek(0x0801200) == 4);
    // assert(tlb_peek(0x0A01200) == 1);
    assert(tlb_translate(0x2301800) == 0x2401800);
    // assert(tlb_peek(0x0001000) == 0);
    // assert(tlb_peek(0x2101000) == 0);
    // assert(tlb_peek(0x2301000) == 1);
    // assert(tlb_peek(0x0501000) == 3);
    // assert(tlb_peek(0x0801000) == 4);
    // assert(tlb_peek(0x0A01000) == 2);
    assert(tlb_translate(0x404000) == 0x424000);
    tlb_clear();
    // assert(tlb_peek(0x301000) == 0);
    // assert(tlb_peek(0x501000) == 0);
    // assert(tlb_peek(0x801000) == 0);
    // assert(tlb_peek(0xA01000) == 0);
    assert(tlb_translate(0xA01200) == 0xA21200);
    puts("");
}

void example3() {
    puts("Example 3:");
    tlb_clear();
    assert(tlb_translate(0xA0001200) == -1);
    // assert(tlb_peek(0xA0001000) == 0);
    assert(tlb_translate(0x1200) == 0x21200);
    // assert(tlb_peek(0xA0001200) == 0);
    // assert(tlb_peek(0x1000) == 1);
    assert(tlb_translate(0xA0001200) == -1);
    assert(tlb_translate(0xB0001200) == -1);
    assert(tlb_translate(0xC0001200) == -1);
    assert(tlb_translate(0xD0001200) == -1);
    assert(tlb_translate(0xE0001200) == -1);
    // assert(tlb_peek(0x1000) == 1);
    assert(tlb_translate(0x1200) == 0x21200);
    puts("");
}

void test1() {
    tlb_clear();
    assert(tlb_translate(0) == 0x0020000);
    assert(tlb_translate(0) == 0x0020000);
    assert(tlb_translate(0) == 0x0020000);
    assert(tlb_translate(0x10000) == 0x0030000);
    assert(tlb_translate(0) == 0x0020000);
}

int main(int argc, char *argv[]) {
    // example1();
    // example2();
    // example3();
    test1();

    printf("Program completed.\n");
    return 0;
}