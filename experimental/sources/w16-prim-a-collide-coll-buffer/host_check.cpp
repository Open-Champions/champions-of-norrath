// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "coll_buffer.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    memset(arena, 0, sizeof arena);
    unsigned table = 0x200;
    memcpy(arena + 0x100 + 536, &table, 4);
    for (unsigned i = 0; i < 6; ++i) {
        unsigned v = 0x1000 + i * 7, junk = 0xdead0000 + i;
        memcpy(arena + table + i * 12, &v, 4);
        memcpy(arena + table + i * 12 + 4, &junk, 4);
        memcpy(arena + table + i * 12 + 8, &junk, 4);
    }
    for (int i : {0, 1, 5})
        if (CollBuffer__9VICollidei(arena + 0x100, i) != 0x1000u + i * 7) return 1;
    return 0;
}
