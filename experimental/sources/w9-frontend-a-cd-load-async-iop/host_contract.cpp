#define NORRATH_HOST_CONTRACT 1
#include "cd_load_async_iop.cpp"

#include <cstdio>
#include <cstring>

signed char cdBusy;
unsigned char* cdResult;
unsigned command[4];

static int lookups, prints, issues;
static int lookup_hit = 1;
static unsigned stored_address = 99, stored_value = 99;
static int stores;
static unsigned issued[4];
static unsigned char* issued_result;

extern "C" void norrath_host_store_absolute(unsigned address, unsigned value) {
    stored_address = address;
    stored_value = value;
    stores += 1;
}
extern "C" int cdDirectoryLookup__FPcPiT1i(char*, int* sector, int* size, int mode) {
    lookups += 1;
    if (mode != 0) return 0;
    *sector = 40;
    *size = 3000;
    return lookup_hit;
}
extern "C" int scePrintf(const char* format, ...) {
    prints += 1;
    return std::strcmp(format, "'%s' not found\n");
}
extern "C" void issueCommand__Fv() {
    issues += 1;
    std::memcpy(issued, command, sizeof issued);
    issued_result = cdResult;
}

int main() {
    char done = 5;
    const int dest = 0x1234560;
    lookup_hit = 0;
    cdLoadAsyncIOP__FPciT0ii(const_cast<char*>("x"), dest, &done, 0, 0);
    if (lookups != 1 || prints != 1 || issues != 0 || cdBusy != 0) return 1;
    if (stores != 1 || stored_address != 0u || stored_value != 0u || done != 5) return 2;
    lookup_hit = 1;
    cdLoadAsyncIOP__FPciT0ii(const_cast<char*>("x"), dest, &done, 0, 2048);
    if (issues != 1 || cdBusy != 1 || done != 0 || issued_result != reinterpret_cast<unsigned char*>(&done)) return 3;
    if (issued[0] != 2u || issued[1] != 41u || issued[2] != (3000u + 2047u) / 2048u) return 4;
    if (issued[3] != static_cast<unsigned>(dest)) return 5;
    cdLoadAsyncIOP__FPciT0ii(const_cast<char*>("x"), dest, &done, 100, -2049);
    if (issued[1] != 38u || issued[2] != 1u) return 6;
    cdLoadAsyncIOP__FPciT0ii(const_cast<char*>("x"), dest, &done, 4097, 2047);
    if (issued[1] != 40u || issued[2] != 3u) return 7;
    if (stores != 1) return 8;
    std::puts("cd load async iop passed");
    return 0;
}
