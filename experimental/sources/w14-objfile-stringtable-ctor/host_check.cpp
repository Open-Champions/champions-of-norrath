// Host check only; not a build input.
#include "ctor.cpp"
#include <cstring>
int main() {
    unsigned o[6];
    std::memset(o, 0x5a, sizeof o);
    __13VIStringTable(o);
    for (int i = 0; i < 4; ++i) if (o[i] != 0) return 1;
    if (o[4] != 0x5a5a5a5au || o[5] != 0x5a5a5a5au) return 2;
    return 0;
}
