// Host check only; not a build input. Words hold arena offsets.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x4000];
unsigned char* const norrath_host_arena_base = arena;
#include "erase.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static std::string trace;
static unsigned* M() { return reinterpret_cast<unsigned*>(arena + 0x100); }
static unsigned* N(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 112 * i); }
extern "C" int Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(const void*, int index) { return index == 2 ? 4 : index == 4 ? 3 : 0; }
extern "C" void EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi(void*, int x) { trace += "F" + std::to_string(x) + ";"; }
extern "C" void FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi(void*, int z) { trace += "Z" + std::to_string(z) + ";"; }
static void link(int i, int flag, int parent, int l, int r) { N(i)[0] = flag; N(i)[1] = parent; N(i)[2] = l; N(i)[3] = r; }
static void tree() {
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; M()[3] = 1;
    // 1 -> left 2, right 5; 2 -> left 6, right 4 (4 -> right 3); successor chain for erasing 2 is 4
    link(0, 7, 7, 7, 7);
    link(1, 1, 0, 2, 5); link(2, 0, 1, 6, 4); link(3, 1, 4, 0, 0);
    link(4, 0, 2, 0, 3); link(5, 1, 1, 0, 0); link(6, 1, 2, 0, 0);
}
int main() {
    bool ok = true;
    // z = 3: a leaf under 4 (its right child); y = z, x = 0 (right child), flag 1 so no fixup
    tree(); trace.clear();
    if (Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(M(), 3) != 0 || N(4)[3] != 0 || N(0)[1] != 4 || trace != "Z3;") ok = false;
    // z = 5: right leaf of the root, flag 1, next returns 0
    tree(); trace.clear();
    if (Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(M(), 5) != 0 || N(1)[3] != 0 || trace != "Z5;") ok = false;
    // z = 4: y = z (one child, left 3), x = 3; flag 0 so the fixup runs with x
    tree(); trace.clear();
    if (Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(M(), 4) != 3 || N(2)[3] != 3 || N(3)[1] != 2 || trace != "F3;Z4;") ok = false;
    // z = 2 (two children): y = next = 4, x = 3, y's flag is 0 so fixup(3); then 4 takes the place of 2
    tree(); trace.clear();
    if (Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(M(), 2) != 4 || trace != "F3;Z2;") ok = false;
    if (N(4)[1] != 1 || N(4)[2] != 6 || N(4)[3] != 3 || N(4)[0] != 0 || N(1)[2] != 4 || N(6)[1] != 4 || N(3)[1] != 4 || N(2)[3] != 3) ok = false;
    if (M()[3] != 1) ok = false;
    // the root word follows y when the erased node was the root: z = 6 is not the root, so use a one-node tree
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; M()[3] = 1; trace.clear();
    link(0, 7, 7, 7, 7); link(1, 1, 0, 0, 2); link(2, 1, 1, 0, 0);
    if (Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(M(), 1) != 0 || M()[3] != 2 || N(2)[1] != 0 || trace != "Z1;") ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
