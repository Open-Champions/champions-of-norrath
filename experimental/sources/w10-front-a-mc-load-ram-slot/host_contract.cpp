#include "mc_load_ram_slot.cpp"

#include <cstdio>

static int calls;
extern "C" void loadRamSlot__Fv() { ++calls; }

int main() {
    MC_LoadRamSlot__Fv();
    if (calls != 1) return 1;
    std::puts("mc load ram slot passed");
    return 0;
}
