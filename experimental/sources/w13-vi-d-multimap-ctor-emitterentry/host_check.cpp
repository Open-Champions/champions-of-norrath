// Host check only; not a build input.
#include "ctor.cpp"
#include <cstring>
int main() {
    unsigned o[8];
    std::memset(o, 0x5a, sizeof o);
    __t10VIMultiMap2ZfZ14VIEmitterEntry(o);
    const unsigned want[5] = {0, 0, 0, 0, 0xffffffffu};
    for (int i = 0; i < 5; ++i) if (o[i] != want[i]) return 1;
    for (int i = 5; i < 8; ++i) if (o[i] != 0x5a5a5a5au) return 2;
    return 0;
}
