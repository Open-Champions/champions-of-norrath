// Host check only; not a build input. Words hold arena offsets.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x4000];
unsigned char* const norrath_host_arena_base = arena;
#include "insert.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static std::string trace;
static unsigned* M() { return reinterpret_cast<unsigned*>(arena + 0x100); }
static unsigned* N(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 112 * i); }
static float& K(int i) { return *reinterpret_cast<float*>(arena + 0x400 + 112 * i + 16); }
static int next_index, add_parent, add_side;
extern "C" int AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii(void*, int parent, int side) {
    add_parent = parent; add_side = side;
    int n = next_index;
    N(n)[0] = 1; N(n)[1] = parent; N(n)[2] = 0; N(n)[3] = 0;
    if (parent == 0) M()[3] = n; else N(parent)[side ? 2 : 3] = n;
    return n;
}
static void rotate(int x, int a, int b) {
    int y = N(x)[a];
    N(x)[a] = N(y)[b];
    if (N(y)[b]) N(N(y)[b])[1] = x;
    int xp = N(x)[1];
    N(y)[1] = xp;
    if (xp == 0) M()[3] = y; else if (N(xp)[2] == (unsigned)x) N(xp)[2] = y; else N(xp)[3] = y;
    N(y)[b] = x; N(x)[1] = y;
}
extern "C" void LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(void*, int i) { trace += "L" + std::to_string(i) + ";"; rotate(i, 3, 2); }
extern "C" void RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(void*, int i) { trace += "R" + std::to_string(i) + ";"; rotate(i, 2, 3); }
extern "C" void __10VIMatrix44RC10VIMatrix44(void* to, const void* from) { std::memcpy(to, from, 64); trace += "M;"; }
static void link(int i, unsigned flag, int parent, int l, int r, float key) { N(i)[0] = flag; N(i)[1] = parent; N(i)[2] = l; N(i)[3] = r; K(i) = key; }
int main() {
    bool ok = true;
    unsigned entry[20];
    for (int i = 0; i < 20; ++i) entry[i] = 0x1000u + i;
    float key = 5.0f;
    // empty map: the new node is the root, gets the key, values and matrix, and ends with flag 0
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; M()[3] = 0; next_index = 1; trace.clear();
    if (Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(M(), &key, entry) != 1) ok = false;
    if (add_parent != 0 || add_side != 0 || M()[3] != 1 || K(1) != 5.0f || N(1)[8] != 0x1000u || N(1)[9] != 0x1001u || N(1)[12] != 0x1004u || N(1)[0] != 0 || trace != "M;") ok = false;
    // equal key goes right; a smaller key goes left
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; M()[3] = 1; link(0, 0, 0, 0, 0, 0); link(1, 0, 0, 0, 0, 5.0f); next_index = 2; trace.clear();
    Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(M(), &key, entry);
    if (add_parent != 1 || add_side != 0 || N(1)[3] != 2) ok = false;
    float low = 1.0f; next_index = 3;
    Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(M(), &low, entry);
    if (add_parent != 1 || add_side != 1 || N(1)[2] != 3) ok = false;
    // red parent and red uncle: recolour only, root ends black
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; M()[3] = 1; trace.clear();
    link(0, 0, 0, 0, 0, 0); link(1, 0, 0, 2, 3, 5.0f); link(2, 1, 1, 0, 0, 2.0f); link(3, 1, 1, 0, 0, 8.0f);
    float k2 = 1.0f; next_index = 4;
    if (Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(M(), &k2, entry) != 4) ok = false;
    if (N(2)[0] != 0 || N(3)[0] != 0 || N(1)[0] != 0 || trace != "M;") ok = false;
    // red parent, black uncle, inner child: LeftRotate(parent) then the final RightRotate(grandparent)
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; M()[3] = 1; trace.clear();
    link(0, 0, 0, 0, 0, 0); link(1, 0, 0, 2, 0, 5.0f); link(2, 1, 1, 0, 0, 2.0f);
    float k3 = 3.0f; next_index = 3;
    Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(M(), &k3, entry);
    if (trace != "M;L2;R1;" || M()[3] != 3 || N(3)[0] != 0 || N(1)[0] != 1 || N(3)[2] != 2 || N(3)[3] != 1) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
