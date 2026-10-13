#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "push_frustum.cpp"
#include <string.h>

static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }

int main() {
    const unsigned stack = 0, entries = 256, vectors = 1024, planes = 4096, camera = 8192;
    for (unsigned i = 0; i < 600; ++i) W(camera + 4 * i) = 0x40000000u + i;
    memset(arena, 0, 64);
    memset(arena + 256, 0xee, 512);
    W(stack + 0) = 2;
    W(stack + 8) = arena_base + entries;
    W(stack + 12) = 7;
    W(stack + 20) = arena_base + vectors;
    W(stack + 24) = 11;
    W(stack + 32) = arena_base + planes;
    memset(arena + vectors, 0xdd, 1024);
    memset(arena + planes, 0xcc, 1024);
    if (PushFrustum__11VIClipStackR8VICamera(arena + stack, arena + camera) != 2) return 1;
    if (W(stack + 0) != 3 || W(stack + 12) != 12 || W(stack + 24) != 16) return 2;
    if (W(stack + 8) != arena_base + entries || W(stack + 20) != arena_base + vectors || W(stack + 32) != arena_base + planes) return 3;
    unsigned e = entries + 2 * 16;
    if (W(e + 0) != 5 || W(e + 4) != 11 || W(e + 8) != 5 || W(e + 12) != 7) return 4;
    if (W(e - 16) != 0xeeeeeeeeu || W(e + 16) != 0xeeeeeeeeu) return 5;
    for (int p = 0; p < 5; ++p)
        for (int k = 0; k < 4; ++k)
            if (W(planes + (11 + p) * 16 + 4 * k) != 0x40000000u + (416 + 16 * p) / 4 + k) return 10 + 4 * p + k;
    const unsigned src[5] = {28, 352, 364, 376, 388};
    for (int v = 0; v < 5; ++v)
        for (int k = 0; k < 3; ++k)
            if (W(vectors + (7 + v) * 12 + 4 * k) != 0x40000000u + src[v] / 4 + k) return 40 + 3 * v + k;
    if (W(planes + 10 * 16) != 0xccccccccu || W(planes + 16 * 16) != 0xccccccccu) return 60;
    if (W(vectors + 6 * 12 + 8) != 0xddddddddu || W(vectors + 12 * 12) != 0xddddddddu) return 61;
    // a second push stacks after the first
    if (PushFrustum__11VIClipStackR8VICamera(arena + stack, arena + camera) != 3) return 62;
    if (W(stack + 24) != 21 || W(stack + 12) != 17 || W(entries + 48 + 4) != 16 || W(entries + 48 + 12) != 12) return 63;
    return 0;
}
