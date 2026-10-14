// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "addtail.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x1000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
unsigned norrath_host_address(const void* pointer) { return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - arena); }
int main() {
    bool ok = true;
    std::memset(arena, 0x5a, sizeof arena);
    unsigned* list = reinterpret_cast<unsigned*>(arena + 0x100);
    list[0] = 0x104; list[1] = 0; list[2] = 0x100;     // an empty list as Init leaves it
    unsigned* a = reinterpret_cast<unsigned*>(arena + 0x200);
    unsigned* b = reinterpret_cast<unsigned*>(arena + 0x300);
    AddTail__7VILListP7VILNode(list, 0);
    if (list[0] != 0x104 || list[1] != 0 || list[2] != 0x100) ok = false;
    AddTail__7VILListP7VILNode(list, a);
    if (list[0] != 0x200 || list[1] != 0 || list[2] != 0x200 || a[0] != 0x104 || a[1] != 0x100) ok = false;
    AddTail__7VILListP7VILNode(list, b);
    if (list[0] != 0x200 || list[2] != 0x300 || a[0] != 0x300 || a[1] != 0x100 || b[0] != 0x104 || b[1] != 0x200) ok = false;
    if (a[2] != 0x5a5a5a5au || b[2] != 0x5a5a5a5au || list[3] != 0x5a5a5a5au) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
