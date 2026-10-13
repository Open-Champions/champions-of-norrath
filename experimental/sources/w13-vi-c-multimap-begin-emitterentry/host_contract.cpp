// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#define NORRATH_HOST_CONTRACT 1
#include <cstring>
static unsigned char table[6 * 112];
extern "C" const unsigned char* norrath_host_pointer(unsigned word) { return word == 0x1000u ? table : 0; }
#include "multimap-begin-emitterentry.cpp"

int main() {
    std::memset(table, 0, sizeof table);
    auto left = [&](int node, int value) { std::memcpy(table + node * 112 + 8, &value, 4); };
    left(1, 3); left(3, 5); left(5, 0); left(2, 4);
    alignas(16) unsigned char map[32] = {};
    unsigned base = 0x1000u;
    std::memcpy(map + 8, &base, 4);
    int root = 0;
    std::memcpy(map + 12, &root, 4);
    if (Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map) != 0) return 1;
    root = 1;
    std::memcpy(map + 12, &root, 4);
    if (Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map) != 5) return 2;
    root = 5;
    std::memcpy(map + 12, &root, 4);
    if (Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map) != 5) return 3;
    root = 2;
    std::memcpy(map + 12, &root, 4);
    if (Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map) != 4) return 4;
    return 0;
}
