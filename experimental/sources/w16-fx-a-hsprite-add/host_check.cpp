// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 14];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "add.cpp"
static int nodeRet, matrixRet, nodeCalls, matrixCalls;
extern "C" int Add__t8VIVector1Z13VIHSpriteNode(void*) { ++nodeCalls; return nodeRet; }
extern "C" int Add__t8VIVector1Z10VIMatrix44(void*) { ++matrixCalls; return matrixRet; }
static int& node(int n, int field) { return *reinterpret_cast<int*>(arena + 256 * n + 4 * field); }
static void reset(unsigned char* o) {
    *reinterpret_cast<unsigned*>(o + 64) = 1;
    *reinterpret_cast<unsigned*>(o + 72) = arena_base;
    for (int n = 0; n < 8; ++n) { node(n, 0) = 0x7777; node(n, 1) = -1; node(n, 2) = -1; }
    nodeCalls = matrixCalls = 0;
}
int main() {
    alignas(8) static unsigned char o[128];
    reset(o); nodeRet = 5; matrixRet = 0;
    if (Add__9VIHSpritei(o, 2) != 5 || node(5, 0) != 2 || node(2, 1) != 5) return 1;
    nodeRet = 6;
    if (Add__9VIHSpritei(o, 2) != 6 || node(2, 1) != 5 || node(5, 2) != 6 || node(6, 0) != 2) return 2;
    nodeRet = 0;
    if (Add__9VIHSpritei(o, 2) != 0 || node(6, 2) != 0 || node(0, 0) != 2) return 3;
    reset(o); nodeRet = -1; matrixRet = 0;
    if (Add__9VIHSpritei(o, 2) != -1 || matrixCalls != 0) return 4;
    nodeRet = 4; matrixRet = -2;
    if (Add__9VIHSpritei(o, 2) != -1 || node(4, 0) != 0x7777 || node(2, 1) != -1) return 5;
    *reinterpret_cast<unsigned*>(o + 64) = 0; nodeCalls = 0;
    if (Add__9VIHSpritei(o, 2) != -1 || nodeCalls != 0) return 6;
    return 0;
}
