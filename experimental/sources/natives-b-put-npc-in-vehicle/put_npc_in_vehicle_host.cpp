#include <cstdio>
#include <cstring>
extern "C" int amxPutNPCInVehicle__FP5__amxPi(void*, int*);
static int vehicle_marker = 1;
static int npc_marker = 2;
static int finds = 0;
static int adds = 0;
static void* added_vehicle = nullptr;
static void* added_npc = nullptr;
static int addresses[2];
static int strings = 0;
static int addresses_seen = 0;
extern "C" int amx_GetAddr(void*, int address, void** physical) {
    static char packed[2][4];
    addresses[addresses_seen] = address;
    *physical = packed[addresses_seen];
    addresses_seen += 1;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, (strings++ & 1) == 0 ? "cart" : "guard");
    return 0;
}
extern "C" void* objectFindObject__FPc(char* name) {
    finds += 1;
    if (std::strcmp(name, "cart") == 0) return &vehicle_marker;
    if (std::strcmp(name, "guard") == 0) return &npc_marker;
    return nullptr;
}
extern "C" void AddNPCDriver__7VehicleP3NPC(void* vehicle, void* npc) {
    adds += 1;
    added_vehicle = vehicle;
    added_npc = npc;
}

int main() {
    int cells[] = {8, 4, 11};
    if (amxPutNPCInVehicle__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (finds != 2 || adds != 1) return 2;
    if (added_vehicle != &vehicle_marker || added_npc != &npc_marker) return 3;
    if (addresses[0] != 4 || addresses[1] != 11) return 4;
    std::puts("amxPutNPCInVehicle passed");
    return 0;
}
