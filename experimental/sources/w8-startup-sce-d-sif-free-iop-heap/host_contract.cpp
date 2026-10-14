#include "sif_free_iop_heap.cpp"

#include <cstdio>

static void* seen;
static int calls;
extern "C" int sceSifFreeSysMemory(void* address) {
    seen = address;
    calls += 1;
    return -77;
}

int main() {
    int probe;
    if (sceSifFreeIopHeap(&probe) != -77 || seen != &probe || calls != 1) return 1;
    if (sceSifFreeIopHeap(nullptr) != -77 || seen != nullptr || calls != 2) return 2;
    std::puts("sif free iop heap contract passed");
    return 0;
}
