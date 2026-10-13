// Host check only; not a build input.
#include "ctor.cpp"
#include <cstring>
int main() {
    unsigned o[6];
    std::memset(o, 0x5a, sizeof o);
    __t7VIArray1Z11VIWorldNode(o);
    if (o[0] != 0 || o[1] != 0) return 1;
    for (int i = 2; i < 6; ++i) if (o[i] != 0x5a5a5a5au) return 2;
    return 0;
}
