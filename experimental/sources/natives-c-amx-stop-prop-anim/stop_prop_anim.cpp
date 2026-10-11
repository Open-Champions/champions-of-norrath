// Observed amxStopPropAnim__FP5__amxPi at 0x0013b380, 60 bytes. Cell 1 is
// an AMX string. objectFindObject finds that name, and byte 88 of the
// object is cleared. The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void* objectFindObject__FPc(char* name);

extern "C" int amxStopPropAnim__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    auto* object = static_cast<unsigned char*>(objectFindObject__FPc(name));
    object[88] = 0;
    return 0;
}
