#include <stdlib.h>

#ifndef CACHEUTIL_H
#define CACHEUTIL_H

size_t get_vpn(size_t va);

size_t get_tag(size_t vpn);

size_t get_index(size_t vpn);

#endif