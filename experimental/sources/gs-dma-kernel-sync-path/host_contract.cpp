#define NORRATH_HOST_CONTRACT 1
#include "sync_path.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned norrath_host_sync_limit_valid = 0;
extern "C" unsigned norrath_host_sync_limit = 0;

static unsigned live[4];
static int remaining[4];
static unsigned vi29_live;
static int vi29_remaining;
static unsigned dump_word;
static int prints;
static char first_line[80];

static int slot(unsigned address) {
    if (address == 0x10009000u) return 0;
    if (address == 0x1000a000u) return 1;
    if (address == 0x10003c00u) return 2;
    if (address == 0x10003020u) return 3;
    return -1;
}

extern "C" unsigned norrath_host_path_word(unsigned address) {
    const int index = slot(address);
    if (index < 0) return dump_word;
    if (remaining[index] == 0) return 0u;
    remaining[index] -= 1;
    return live[index];
}
extern "C" unsigned norrath_host_path_vi29() {
    if (vi29_remaining == 0) return 0u;
    vi29_remaining -= 1;
    return vi29_live;
}
extern "C" int scePrintf(const char* text, ...) {
    if (prints == 0) std::snprintf(first_line, sizeof first_line, "%s", text);
    prints += 1;
    return 0;
}

static void clear() {
    std::memset(live, 0, sizeof live);
    std::memset(remaining, 0, sizeof remaining);
    vi29_live = 0;
    vi29_remaining = 0;
    dump_word = 0xabcdu;
    prints = 0;
    first_line[0] = 0;
}

int main() {
    if (norrath_gs_sync_path_limit != 0x01000000u) return 1;
    clear();
    live[0] = 0x100u;
    live[1] = 0x100u;
    live[2] = 0x1f000003u;
    live[3] = 0xc00u;
    vi29_live = 0x100u;
    remaining[0] = remaining[1] = remaining[2] = remaining[3] = 1;
    vi29_remaining = 1;
    if (sceGsSyncPath(1, 99) != 0x1f || prints != 0) return 2;
    clear();
    live[2] = 0x00e00000u;
    remaining[2] = 1;
    if (sceGsSyncPath(4, 0) != 0) return 3;

    norrath_host_sync_limit_valid = 1;
    norrath_host_sync_limit = 0;
    clear();
    if (sceGsSyncPath(0, 7) != 0 || prints != 0) return 4;

    live[0] = 0x100u;
    remaining[0] = 1;
    if (sceGsSyncPath(0, 0) != 0 || prints != 0) return 5;

    clear();
    live[0] = 0x100u;
    remaining[0] = 8;
    if (sceGsSyncPath(0, 0) != -1 || prints != 11) return 6;
    if (std::strcmp(first_line, "sceGsSyncPath: DMA Ch.1 does not terminate\r\n") != 0) return 7;

    clear();
    live[0] = 0x100u;
    remaining[0] = 1;
    live[1] = 0x100u;
    remaining[1] = 8;
    if (sceGsSyncPath(0, 0) != -1) return 8;
    if (std::strcmp(first_line, "sceGsSyncPath: DMA Ch.2 does not terminate\r\n") != 0) return 9;
    std::puts("sync path contract passed");
    return 0;
}
