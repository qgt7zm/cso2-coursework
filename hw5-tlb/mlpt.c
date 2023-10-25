#include "config.h"
#include "mlpt.h"

// Page Table Functions

size_t translate(size_t va) {
    if (va < 0x1234000){
        return va + 0x20000;
    } else if (va > 0x2000000 && va < 0x2345000){
        return va + 0x100000;
    } else {
        return -1;
    }
}

void page_allocate(size_t va) {
}

void page_deallocate(size_t va) {
}