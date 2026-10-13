// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "multimap-end-emitterentry.cpp"

int main() {
    alignas(8) unsigned char map[64] = {1, 2, 3};
    if (End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map) != 0) return 1;
    if (End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(0) != 0) return 2;
    return 0;
}
