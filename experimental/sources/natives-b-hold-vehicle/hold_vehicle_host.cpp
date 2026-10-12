#include <cstdio>
#include <cstring>
extern "C" int amxHoldVehicle__FP5__amxPi(void*, int*);
static int marker = 1;
static int holds = 0;
static void* held;
static int hold_value = -1;
static int find_null = 0;
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "cart");
    return 0;
}
extern "C" void* objectFindObject__FPc(char*) {
    return find_null ? nullptr : &marker;
}
extern "C" void SetHold__7Vehiclei(void* vehicle, int hold) {
    holds += 1;
    held = vehicle;
    hold_value = hold;
}

int main() {
    int cells[] = {8, 3, 6};
    find_null = 1;
    if (amxHoldVehicle__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (holds != 0) return 2;
    find_null = 0;
    if (amxHoldVehicle__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (holds != 1 || held != &marker || hold_value != 6) return 4;
    std::puts("amxHoldVehicle passed");
    return 0;
}
