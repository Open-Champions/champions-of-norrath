// Host check only; not a build input.
#include "destroypool.cpp"
#include <cstring>
int main() {
    unsigned o[4];
    const unsigned counts[] = {0u, 1u, 5u};
    for (unsigned n : counts) {
        std::memset(o, 0x5a, sizeof o);
        o[0] = n;
        if (DestroyPool__t7VIArray1Z11VIWorldNode(o) != 0) return 1;
        if (o[0] != n || o[1] != 0x5a5a5a5au || o[2] != 0x5a5a5a5au) return 2;
    }
    return 0;
}
