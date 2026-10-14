// Observed amxPropAnimAtEnd at 0x0013b3c0, 64 bytes. It finds the
// object named by the first AMX cell and returns bit 7 of the
// halfword at object+90.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void* objectFindObject__FPc(char* name);

extern "C" int amxPropAnimAtEnd__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    unsigned char* object = static_cast<unsigned char*>(objectFindObject__FPc(text));
    const unsigned half = static_cast<unsigned>(object[90])
        | (static_cast<unsigned>(object[91]) << 8);
    return static_cast<int>((half >> 7) & 1u);
}
