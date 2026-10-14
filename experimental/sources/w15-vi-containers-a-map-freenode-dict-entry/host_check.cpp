// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "freenode.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned* node(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 28 * i); }
int main() {
    bool ok = true;
    std::memset(arena, 0x5a, sizeof arena);
    unsigned* m = reinterpret_cast<unsigned*>(arena + 0x100);
    m[0] = 5; m[2] = 0x400; m[4] = 7;
    if (FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(m, 3) != 0) ok = false;
    if (m[0] != 4 || m[2] != 0x400 || m[4] != 3 || node(3)[1] != 7 || node(3)[2] != 0xfffffffeu) ok = false;
    // untouched neighbours and the other node words
    if (node(3)[0] != 0x5a5a5a5au || node(3)[3] != 0x5a5a5a5au || node(2)[1] != 0x5a5a5a5au || node(4)[2] != 0x5a5a5a5au) ok = false;
    if (m[1] != 0x5a5a5a5au || m[3] != 0x5a5a5a5au) ok = false;
    // chain a second node on the first
    FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(m, 1);
    if (m[0] != 3 || m[4] != 1 || node(1)[1] != 3 || node(1)[2] != 0xfffffffeu) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
