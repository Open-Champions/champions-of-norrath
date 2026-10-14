// Host check only; not a build input. Addresses are compared as 32-bit values.
#include "init.cpp"
#include <cstring>
int main() {
    alignas(16) unsigned l[8];
    std::memset(l, 0x5a, sizeof l);
    Init__7VILList(l + 1);
    unsigned a = static_cast<unsigned>(reinterpret_cast<unsigned long>(l + 1));
    if (l[0] != 0x5a5a5a5au || l[1] != a + 4 || l[2] != 0 || l[3] != a || l[4] != 0x5a5a5a5au) return 1;
    return 0;
}
