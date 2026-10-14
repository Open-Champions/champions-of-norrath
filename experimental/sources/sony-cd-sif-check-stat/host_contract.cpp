#include <cstdio>
#include <cstdint>
#define NORRATH_HOST_CONTRACT 1
#include "check_stat.cpp"
static unsigned char other_bytes[28];
extern "C" const unsigned char* norrath_sony_cd_resolve(unsigned bits) {
    if (bits != 1u) return nullptr;
    return other_bytes;
}

static void store(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    unsigned char client[8] = {};
    unsigned char* other = other_bytes;
    if (sceSifCheckStatRpc(client) != 0) return 1;
    store(client, 1u);
    store(client + 4, 7u);
    store(other + 24, 8u);
    store(other + 16, 1u);
    if (sceSifCheckStatRpc(client) != 0) return 2;
    store(other + 24, 7u);
    store(other + 16, 0u);
    if (sceSifCheckStatRpc(client) != 0) return 3;
    store(other + 16, 0x11u);
    if (sceSifCheckStatRpc(client) != 1) return 4;
    store(other + 16, 2u);
    if (sceSifCheckStatRpc(client) != 0) return 5;
    std::puts("sif check stat contract passed");
    return 0;
}
