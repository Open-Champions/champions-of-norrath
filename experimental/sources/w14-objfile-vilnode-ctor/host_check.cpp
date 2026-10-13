// Host check only; not a build input.
#include "ctor.cpp"
#include <cstring>
int main() {
    unsigned n[4];
    std::memset(n, 0x5a, sizeof n);
    __7VILNode(n);
    if (n[0] != 0 || n[1] != 0 || n[2] != 0x5a5a5a5au || n[3] != 0x5a5a5a5au) return 1;
    return 0;
}
