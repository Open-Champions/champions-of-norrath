// Host contract for getBestSlotName__FPc. Not an EE plan input.
#include "get_best_slot_name.cpp"
#include <cstdio>
#include <cstring>
char sortedDir[64 * 70];
int sortedDirCount;
static void setSlot(int index, int slot) {
    std::snprintf(sortedDir + 64 * index + 45, 19, "slot%d", slot);
}
int main() {
    char out[64];
    sortedDirCount = 0;
    getBestSlotName__FPc(out);
    if (std::strcmp(out, "BASLUS-20565_slot0") != 0) return 1;
    setSlot(0, 0); setSlot(1, 2); sortedDirCount = 2;
    getBestSlotName__FPc(out);
    if (std::strcmp(out, "BASLUS-20565_slot1") != 0) return 2;
    setSlot(1, 1); sortedDirCount = 2;
    getBestSlotName__FPc(out);
    if (std::strcmp(out, "BASLUS-20565_slot2") != 0) return 3;
    for (int i = 0; i < 64; ++i) setSlot(i, 63 - i);
    sortedDirCount = 64;
    getBestSlotName__FPc(out);
    if (std::strcmp(out, "BASLUS-20565_slot64") != 0) return 4;
    sortedDirCount = 63;                      // slot 0 is held by record 63, which is not counted
    getBestSlotName__FPc(out);
    if (std::strcmp(out, "BASLUS-20565_slot0") != 0) return 5;
    return 0;
}
