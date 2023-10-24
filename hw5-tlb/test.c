#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include "config.h"
#include "mlpt.h"
#include "tlb.h"

#include "cacheutil.h"

void test(size_t va) {
    printf("VA = 0x%lx\n", va);

    size_t vpn = get_vpn(va);
    printf("VPN = 0x%lx\n", vpn);

    size_t tag = get_tag(vpn);
    printf("Tag = 0x%lx\n", tag);

    size_t index = get_index(vpn);
    printf("Index = 0x%lx\n", index);
}

int main(int argc, char *argv[]) {
    test(0xabc123);

    printf("Program completed.\n");
    return 0;
}