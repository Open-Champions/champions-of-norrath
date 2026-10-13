#include "mc_get_load_name.cpp"

#include <cstdio>

char loadName[128];

int main() {
    if (MC_GetLoadName__Fv() != loadName) return 1;
    std::puts("mc get load name passed");
    return 0;
}
