#define NORRATH_HOST_CONTRACT 1
#include "iop_heap.cpp"

#include <cstdio>

extern "C" int _bind = -1;
int norrath_iop_heap_wait_steps;

static int bind_results[4];
static unsigned servers[4];
static int modes[4];
static void* clients[4];
static int binds;
static int plant;

extern "C" int sceSifBindRpc(void* client, unsigned server, int mode) {
    clients[binds] = client;
    servers[binds] = server;
    modes[binds] = mode;
    auto* bytes = static_cast<unsigned char*>(client);
    unsigned value = 0;
    if (plant == 1) value = 0x11u;
    if (plant == 2) value = binds == 0 ? 0u : 0x11u;
    bytes[36] = static_cast<unsigned char>(value);
    bytes[37] = static_cast<unsigned char>(value >> 8);
    bytes[38] = static_cast<unsigned char>(value >> 16);
    bytes[39] = static_cast<unsigned char>(value >> 24);
    const int result = bind_results[binds];
    ++binds;
    return result;
}

int main() {
    if (sizeof norrath_iop_heap_client != 64) return 1;
    bind_results[0] = -5;
    plant = 0;
    if (sceSifInitIopHeap() != -1 || binds != 1 || _bind != -1 || norrath_iop_heap_wait_steps != 0) return 2;
    if (servers[0] != 0x80000003u || modes[0] != 0 || clients[0] != norrath_iop_heap_client) return 3;

    binds = 0;
    norrath_iop_heap_wait_steps = 0;
    _bind = -1;
    plant = 1;
    bind_results[0] = 4;
    if (sceSifInitIopHeap() != 0 || binds != 1 || _bind != 0 || norrath_iop_heap_wait_steps != 0) return 4;

    binds = 0;
    norrath_iop_heap_wait_steps = 0;
    _bind = 8;
    plant = 2;
    bind_results[0] = 0;
    bind_results[1] = 2;
    if (sceSifInitIopHeap() != 0 || binds != 2 || _bind != 0) return 5;
    if (norrath_iop_heap_wait_steps != 0x100001 || clients[1] != norrath_iop_heap_client || modes[1] != 0) return 6;
    std::puts("sceSifInitIopHeap passed");
    return 0;
}
