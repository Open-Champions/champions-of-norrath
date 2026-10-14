// Observed amxHoldVehicle at 0x0013acb0, 84 bytes. It finds the object
// named by the first AMX cell. A found object is passed to SetHold
// with the second cell. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void* objectFindObject__FPc(char* name);
extern "C" void SetHold__7Vehiclei(void* vehicle, int hold);

extern "C" int amxHoldVehicle__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    void* vehicle = objectFindObject__FPc(text);
    if (vehicle != nullptr) SetHold__7Vehiclei(vehicle, params[2]);
    return 0;
}
