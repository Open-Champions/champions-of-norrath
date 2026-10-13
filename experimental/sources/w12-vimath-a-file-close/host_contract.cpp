#define NORRATH_HOST_ARENA
#include <string.h>
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return address == 0 ? nullptr : arena + (address - arena_base); }
#include "close.cpp"

static int order[16], n, async_calls, async_until, deletes, printed;
static void* deleted[2];
static char message[32];
static unsigned flags_at_flush;
extern "C" int printf(const char* f, ...) { order[n++] = 2; ++printed; strncpy(message, f, 31); return 0; }
extern "C" void BufferFlush__6VIFile(void*) { order[n++] = 1; flags_at_flush = *reinterpret_cast<unsigned*>(arena); }
extern "C" int IsAsynchComplete__6VIFile(void*) { order[n++] = 3; return ++async_calls >= async_until; }
extern "C" void __builtin_vec_delete(void* p) { order[n++] = 4; deleted[deletes++ & 1] = p; }

static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static void setup(unsigned handle, unsigned first, unsigned second) {
    memset(arena, 0xee, sizeof arena);
    n = async_calls = deletes = printed = 0; async_until = 3;
    W(0) = handle; W(184) = first; W(56) = second;
}
static int zeroed() {
    static const int words[] = {4, 8, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 60, 64, 68, 112};
    for (int w : words) if (W(w) != 0) return w;
    return 0;
}

int main() {
    setup(5, arena_base + 512, arena_base + 768);
    if (Close__6VIFile(arena) != 0) return 1;
    if (order[0] != 1 || order[1] != 2 || n != 4 || order[2] != 4 || order[3] != 4) return 2;
    if (strcmp(message, "VIEEFile: Close\n") != 0 || flags_at_flush != 5) return 3;
    if (static_cast<int>(W(0)) != -1) return 4;
    if (zeroed()) return 5;
    if (W(184) != 0 || W(188) != 0 || W(56) != 0) return 6;
    if (deleted[0] != arena + 512 || deleted[1] != arena + 768) return 7;
    if (W(12) != 0xeeeeeeeeu || W(72) != 0xeeeeeeeeu || W(180) != 0xeeeeeeeeu || W(192) != 0xeeeeeeeeu || W(116) != 0xeeeeeeeeu) return 8;
    // asynchronous bit set: waits until complete
    setup(0x45, 0, 0);
    if (Close__6VIFile(arena) != 0 || async_calls != 3 || n != 2 + 3 || deletes != 0) return 9;
    if (static_cast<int>(W(0)) != -1 || zeroed()) return 10;
    if (W(184) != 0 || W(188) != 0xeeeeeeeeu || W(56) != 0) return 11;   // nothing to release: the second word is left alone
    // handle already negative: no flush/print, no wait, fields still cleared
    setup(0xffffffffu, arena_base + 512, 0);
    if (Close__6VIFile(arena) != 0 || printed != 0 || order[0] != 4 || n != 1 || static_cast<int>(W(0)) != -1) return 12;
    if (zeroed() || W(184) != 0) return 13;
    return 0;
}
