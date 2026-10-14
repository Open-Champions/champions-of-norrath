// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "erase.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::string trace; static int next_result;
static unsigned* M() { return reinterpret_cast<unsigned*>(arena + 0x100); }
static unsigned* N(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 28 * i); }
extern "C" int Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(const void*, int it) { trace += "next" + std::to_string(it) + ";"; return next_result; }
extern "C" int EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void*, int it) { trace += "fix" + std::to_string(it) + ";"; return 0; }
extern "C" int FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void*, int it) { trace += "free" + std::to_string(it) + ";"; return 0; }
// fields: [0] colour-like, [1] parent, [2] left, [3] right
static void set(int i, unsigned c, unsigned p, unsigned l, unsigned r) { N(i)[0] = c; N(i)[1] = p; N(i)[2] = l; N(i)[3] = r; }
static void tree() {
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; M()[3] = 1;
    set(0, 7, 7, 7, 7);
    set(1, 1, 0, 2, 3);    // root, two children
    set(2, 0, 1, 0, 0);    // leaf, zero word
    set(3, 1, 1, 4, 0);    // right child with a left child
    set(4, 0, 3, 0, 0);
}
int main() {
    bool ok = true;
    // leaf with zero colour word: unlinked from its parent, fixup with x = 0, nothing else
    tree(); trace.clear(); next_result = 1;
    int r = Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(M(), 2);
    if (r != 1 || trace != "next2;fix0;free2;" || N(1)[2] != 0 || N(1)[3] != 3 || N(0)[1] != 1 || M()[3] != 1) ok = false;
    // node with one child (3 has only a left child): the child takes its place, no fixup (word nonzero)
    tree(); trace.clear(); next_result = 1;
    r = Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(M(), 3);
    if (r != 1 || trace != "next3;free3;" || N(1)[3] != 4 || N(4)[1] != 1) ok = false;
    // root with two children, successor 3: node 3 takes the root's place
    tree(); trace.clear(); next_result = 3;
    r = Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(M(), 1);
    if (r != 3 || trace != "next1;free1;" || M()[3] != 3) ok = false;
    // successor 3 had a left child 4 which moved up: x = 4 with parent 1 (the root), then 3 copies the root's links
    if (N(3)[1] != 0 || N(3)[2] != 2 || N(3)[3] != 4 || N(3)[0] != 1) ok = false;
    if (N(2)[1] != 3 || N(4)[1] != 3) ok = false;
    // root alone: the root word becomes 0
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; M()[3] = 1; set(0, 7, 7, 7, 7); set(1, 0, 0, 0, 0);
    trace.clear(); next_result = 0;
    r = Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(M(), 1);
    if (r != 0 || trace != "next1;fix0;free1;" || M()[3] != 0 || N(0)[1] != 0) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
