// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "pool-begin-particledefinition.cpp"
#include <cstring>
#include <initializer_list>

static const void* end_arg;
extern "C" int End__Ct6VIPool1ZP20VIParticleDefinition(const void* pool) { end_arg = pool; return 0x7fffffff; }

int main() {
    alignas(8) unsigned char pool[64];
    for (int first : {0, 1, 7, -2, 0x12345678}) {
        std::memset(pool, 0x5c, sizeof pool);
        std::memcpy(pool + 16, &first, 4);
        end_arg = 0;
        if (Begin__Ct6VIPool1ZP20VIParticleDefinition(pool) != first) return 1;
        if (end_arg != 0) return 2;
    }
    int none = -1;
    std::memcpy(pool + 16, &none, 4);
    if (Begin__Ct6VIPool1ZP20VIParticleDefinition(pool) != 0x7fffffff) return 3;
    if (end_arg != pool) return 4;
    return 0;
}
