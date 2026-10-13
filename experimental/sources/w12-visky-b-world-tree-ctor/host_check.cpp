#include "ctor.cpp"

static int order[4];
static void* seen[4];
static int n;
extern "C" void __t7VIArray1Z11VIWorldNode(void* p) { order[n] = 1; seen[n++] = p; }
extern "C" void __t7VIArray1Z11VIWorldLeaf(void* p) { order[n] = 2; seen[n++] = p; }

int main() {
    alignas(8) static unsigned char object[32];
    for (int i = 0; i < 32; ++i) object[i] = 0xee;
    __11VIWorldTree(object);
    if (n != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (seen[0] != object || seen[1] != object + 8) return 2;
    if (*reinterpret_cast<int*>(object + 16) != 1) return 3;
    for (int i = 20; i < 32; ++i) if (object[i] != 0xee) return 5;
    return 0;
}
