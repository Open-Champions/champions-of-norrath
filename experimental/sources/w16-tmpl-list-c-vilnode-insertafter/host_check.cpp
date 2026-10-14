// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x2000];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "insert_after.cpp"
#include <cstdio>
#include <cstring>
static unsigned addr(unsigned off) { return arena_base + off; }
static unsigned* at(unsigned off) { return reinterpret_cast<unsigned*>(arena + off); }
int main() {
    bool ok = true;
    // forward chain r -> s, with s -> 0; insert n after r
    std::memset(arena, 0x5a, sizeof arena);
    at(0x100)[0] = addr(0x200); at(0x100)[1] = 0;
    at(0x200)[0] = 0;           at(0x200)[1] = addr(0x100);
    InsertAfter__7VILNodeP7VILNode(addr(0x300), addr(0x100));
    if (at(0x300)[1] != addr(0x100) || at(0x300)[0] != addr(0x200) || at(0x200)[1] != addr(0x300) || at(0x100)[0] != addr(0x300)) ok = false;
    if (at(0x100)[1] != 0 || at(0x200)[0] != 0) ok = false;
    // reference with a zero forward link: p -> e (e is the end marker, e+0 == 0, e+4 == p)
    std::memset(arena, 0x5a, sizeof arena);
    at(0x100)[0] = addr(0x200); at(0x100)[1] = 0;
    at(0x200)[0] = 0;           at(0x200)[1] = addr(0x100);
    InsertAfter__7VILNodeP7VILNode(addr(0x300), addr(0x200));
    if (at(0x300)[1] != addr(0x100) || at(0x300)[0] != addr(0x200) || at(0x200)[1] != addr(0x300) || at(0x100)[0] != addr(0x300)) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
