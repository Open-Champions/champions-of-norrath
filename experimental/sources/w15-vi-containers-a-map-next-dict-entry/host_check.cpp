// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "next.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned* node(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 28 * i); }
int main() {
    bool ok = true;
    std::memset(arena, 0x5a, sizeof arena);
    unsigned* m = reinterpret_cast<unsigned*>(arena + 0x100);
    m[2] = 0x400;
    // Nodes are set as (parent, left, right):
    //        1
    //      /   \
    //     2     3
    //      \   /
    //       4 5
    //        \
    //         6
    for (int i = 0; i < 8; ++i) { node(i)[1] = 0; node(i)[2] = 0; node(i)[3] = 0; }
    node(1)[2] = 2; node(1)[3] = 3;
    node(2)[1] = 1; node(2)[3] = 4;
    node(3)[1] = 1; node(3)[2] = 5;
    node(4)[1] = 2; node(4)[3] = 6;
    node(5)[1] = 3;
    node(6)[1] = 4;
    // In-order: 2 4 6 1 5 3
    struct { int from; int want; } cases[] = {{2, 4}, {4, 6}, {6, 1}, {1, 5}, {5, 3}, {3, 0}};
    for (auto& c : cases) if (Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(m, c.from) != c.want) ok = false;
    // Node 0 is a real record: a zero right and a zero parent give zero.
    if (Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(m, 0) != 0) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
