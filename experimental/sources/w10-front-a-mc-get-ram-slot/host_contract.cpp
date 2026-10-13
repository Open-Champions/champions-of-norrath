#include "mc_get_ram_slot.cpp"

#include <cstdio>

unsigned char ramSlot[16];

int main() {
    int size = 5;
    if (MC_GetRamSlot__FPi(&size) != ramSlot || size != 731592) return 1;
    if (MC_GetRamSlot__FPi(nullptr) != ramSlot) return 2;
    std::puts("mc get ram slot passed");
    return 0;
}
