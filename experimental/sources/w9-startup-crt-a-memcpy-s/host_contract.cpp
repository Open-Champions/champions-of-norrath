// The library memcpy is renamed so the host's own copy is not interposed.
#define memcpy norrath_host_memcpy
#include "memcpy_s.cpp"
#undef memcpy

#include <cstdio>
#include <cstring>

static int copies;
static void* seen_destination;
static const void* seen_source;
static unsigned seen_size;

extern "C" void* norrath_host_memcpy(void* destination, const void* source, unsigned size) {
    ++copies;
    seen_destination = destination;
    seen_source = source;
    seen_size = size;
    return std::memcpy(destination, source, size);
}

int main() {
    alignas(4) unsigned char object[64] = {};
    unsigned char source[64];
    unsigned char destination[64] = {};
    for (int i = 0; i < 64; ++i) source[i] = static_cast<unsigned char>(i + 1);
    *reinterpret_cast<int*>(object + 48) = 32;

    if (MemcpyS(object, destination, source, 0, 32) != 0 || copies != 1) return 1;
    if (seen_destination != destination || seen_source != source || seen_size != 32u) return 2;
    if (std::memcmp(destination, source, 32) != 0 || destination[32] != 0) return 3;

    // The offset counts words and reduces the room: 32 - 4 * 3 = 20.
    copies = 0;
    std::memset(destination, 0, sizeof destination);
    if (MemcpyS(object, destination, source, 3, 20) != 0 || copies != 1) return 4;
    if (seen_source != source + 12 || destination[0] != 13 || destination[19] != 32) return 5;
    if (MemcpyS(object, destination, source, 3, 21) != -543 || copies != 1) return 6;

    if (MemcpyS(object, destination, source, 0, 33) != -543 || copies != 1) return 7;
    if (MemcpyS(object, destination, source, 0, -1) != -543 || copies != 1) return 8;
    // A zero size is accepted and still reaches memcpy.
    if (MemcpyS(object, destination, source, 0, 0) != 0 || copies != 2 || seen_size != 0u) return 9;
    // A null destination copies nothing and succeeds, even when the size is bad.
    if (MemcpyS(object, nullptr, source, 0, 1000) != 0 || copies != 2) return 10;
    // A negative limit rejects any size.
    *reinterpret_cast<int*>(object + 48) = -4;
    if (MemcpyS(object, destination, source, 0, 0) != -543) return 11;
    std::puts("memcpy s contract passed");
    return 0;
}
