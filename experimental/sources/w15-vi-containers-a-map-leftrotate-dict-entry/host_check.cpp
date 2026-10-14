// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "leftrotate.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned* node(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 28 * i); }
static unsigned* map() { return reinterpret_cast<unsigned*>(arena + 0x100); }
// Subtree rooted at X = 2 (under 1 = root on the near side) with Y the child on the rotating side and an inner grandchild G.
static void build(bool xIsLeftChild, bool withInner) {
    std::memset(arena, 0x5a, sizeof arena);
    for (int i = 0; i < 8; ++i) { node(i)[1] = 0; node(i)[2] = 0; node(i)[3] = 0; }
    map()[2] = 0x400; map()[3] = 1;
    node(1)[xIsLeftChild ? 2 : 3] = 2; node(2)[1] = 1;
    const int up = 3;
    node(2)[up] = 3; node(3)[1] = 2;
    if (withInner) { node(3)[up == 3 ? 2 : 3] = 4; node(4)[1] = 3; }
    node(2)[up == 3 ? 2 : 3] = 5; node(5)[1] = 2;     // the other child of X
    node(3)[up] = 6; node(6)[1] = 3;                  // Y's outer child
}
int main() {
    bool ok = true;
    for (int variant = 0; variant < 4; ++variant) {
        const bool xLeft = variant & 1, inner = variant & 2;
        build(xLeft, inner);
        const int up = 3, down = up == 3 ? 2 : 3;
        if (LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map(), 2) != 0) ok = false;
        // Y = 3 is now X's parent and takes X's place under node 1.
        if (node(3)[1] != 1 || node(2)[1] != 3 || node(3)[down] != 2) ok = false;
        if (node(1)[xLeft ? 2 : 3] != 3 || node(1)[xLeft ? 3 : 2] != 0 || map()[3] != 1) ok = false;
        if (node(2)[up] != (inner ? 4u : 0u)) ok = false;
        if (inner && node(4)[1] != 2) ok = false;
        if (node(2)[down] != 5 || node(5)[1] != 2 || node(3)[up] != 6 || node(6)[1] != 3) ok = false;
        // Untouched words (colour/key area of the nodes).
        if (node(2)[0] != 0x5a5a5a5au || node(3)[4] != 0x5a5a5a5au) ok = false;
    }
    // X at the root: the root word becomes Y.
    build(true, true);
    node(1)[1] = 0; node(1)[2] = 0; node(1)[3] = 0; map()[3] = 2; node(2)[1] = 0;
    if (LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map(), 2) != 0 || map()[3] != 3 || node(3)[1] != 0 || node(2)[1] != 3) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
