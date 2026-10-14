// Host check only; not a build input.
#include "ctor.cpp"
#include <cstring>
int main() {
    unsigned o[4];
    std::memset(o, 0x5a, sizeof o);
    __t7VIArray1Zc(o);
    if (o[0] != 0 || o[1] != 0) return 1;
    if (o[2] != 0x5a5a5a5au || o[3] != 0x5a5a5a5au) return 2;
    return 0;
}
