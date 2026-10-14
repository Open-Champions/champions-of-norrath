// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "pool-end-particledefinition.cpp"

int main() {
    alignas(8) unsigned char pool[64] = {0};
    if (End__Ct6VIPool1ZP20VIParticleDefinition(pool) != 0x7fffffff) return 1;
    if (End__Ct6VIPool1ZP20VIParticleDefinition(0) != 0x7fffffff) return 2;
    return 0;
}
