// Observed amxPutNPCInVehicle at 0x0013aac0, 156 bytes. It resolves
// the first AMX cell to a vehicle and, when that object exists, the
// second cell to an NPC. AddNPCDriver receives both, including a null
// NPC. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void* objectFindObject__FPc(char* name);
extern "C" void AddNPCDriver__7VehicleP3NPC(void* vehicle, void* npc);

extern "C" int amxPutNPCInVehicle__FP5__amxPi(void* amx, int* params) {
    char vehicle_name[160];
    char npc_name[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(vehicle_name, cells);
    (void)amx_GetAddr(amx, params[2], &cells);
    (void)amx_GetString(npc_name, cells);
    void* vehicle = objectFindObject__FPc(vehicle_name);
    if (vehicle != nullptr) {
        void* npc = objectFindObject__FPc(npc_name);
        AddNPCDriver__7VehicleP3NPC(vehicle, npc);
    }
    return 0;
}
