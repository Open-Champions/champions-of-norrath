// Host check: build with -fno-builtin so the memset below is the one called.
#include "mc_reset_ram_slot.cpp"

#include <cstdio>

signed char MC_ProtectRamSlot;
char ramSlot[0xb29c8 + 8];
static void* seen_dest;
static int seen_value;
static unsigned long seen_count;
static int calls;

extern "C" void* memset(void* dest, int value, unsigned long count) {
    ++calls;
    seen_dest = dest;
    seen_value = value;
    seen_count = count;
    char* p = static_cast<char*>(dest);
    for (unsigned long i = 0; i < count; ++i) p[i] = static_cast<char>(value);
    return dest;
}

int main() {
    for (char& c : ramSlot) c = 0x5a;
    MC_ProtectRamSlot = 1;
    MC_ResetRamSlot__Fv();
    if (calls != 0 || ramSlot[0] != 0x5a) return 1;
    MC_ProtectRamSlot = 0;
    MC_ResetRamSlot__Fv();
    if (calls != 1 || seen_dest != ramSlot || seen_value != 0 || seen_count != 0xb29c8) return 2;
    if (ramSlot[0] != 0 || ramSlot[0xb29c7] != 0 || ramSlot[0xb29c8] != 0x5a) return 3;
    std::puts("mc reset ram slot passed");
    return 0;
}
