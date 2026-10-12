#include "mc_spawn_point.cpp"

#include <cstdio>

int main() {
    if (sizeof MC_SpawnPoint != 48) return 1;
    for (unsigned i = 0; i < sizeof MC_SpawnPoint; ++i) if (MC_SpawnPoint[i] != 0) return 2;
    std::puts("mc spawn point passed");
    return 0;
}
