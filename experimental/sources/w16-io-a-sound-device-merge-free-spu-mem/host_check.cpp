// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static int removed[8], removedCount, tailed[8], tailedCount;
static unsigned lastTailList;
extern "C" void Remove__7VILNode(unsigned node) { removed[removedCount++] = static_cast<int>(node); }
extern "C" void AddTail__7VILListP7VILNode(unsigned char* list, unsigned node) {
    lastTailList = static_cast<unsigned>(list - arena);
    tailed[tailedCount++] = static_cast<int>(node);
}
#include "merge_free_spu_mem.cpp"
static unsigned& w(unsigned address, int offset) { return *reinterpret_cast<unsigned*>(arena + address + offset); }
int main() {
    unsigned device = 0x1000;
    // three nodes at 0x2000, 0x2100, 0x2200 plus an end node at 0x2300
    w(device, 3312) = 0x2000;
    for (int i = 0; i < 4; ++i) w(0x2000 + 0x100 * i, 0) = i < 3 ? 0x2000 + 0x100 * (i + 1) : 0;
    w(0x2000, 16) = 0x100; w(0x2000, 20) = 0x40;
    w(0x2100, 16) = 0x140; w(0x2100, 20) = 0x80;
    w(0x2200, 16) = 0x400; w(0x2200, 20) = 0x40;
    MergeFreeSpuMem__13VISoundDevice(arena + device);
    if (removedCount != 1 || removed[0] != 0x2000 || tailedCount != 1 || tailed[0] != 0x2000 || lastTailList != device + 3324) return 1;
    if (w(0x2100, 16) != 0x100 || w(0x2100, 20) != 0xc0) return 2;
    if (w(0x2200, 16) != 0x400 || w(0x2200, 20) != 0x40) return 3;
    // an empty list (first node is the end node) does nothing
    removedCount = tailedCount = 0;
    w(device, 3312) = 0x2300;
    MergeFreeSpuMem__13VISoundDevice(arena + device);
    if (removedCount != 0 || tailedCount != 0) return 4;
    // two blocks that are not adjacent are left alone
    w(device, 3312) = 0x2100;
    MergeFreeSpuMem__13VISoundDevice(arena + device);
    if (removedCount != 0 || w(0x2200, 16) != 0x400) return 5;
    return 0;
}
