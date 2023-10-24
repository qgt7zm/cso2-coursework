#include <stdio.h>

#include "config.h"
#include "mlpt.h"
#include "tlb.h"

void tlb_clear() {
}

int tlb_peek(size_t va) {
    return 0;
}

size_t tlb_translate(size_t va) {
    // TODO read PA if incache

    // Virtual address is not in cache
    return translate(va);
}