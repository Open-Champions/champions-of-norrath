// amxControlVehicle is the 96-byte native at 0x0013ab60. Cell 1 names an
// object. A null object returns zero. Otherwise cell 2 is stored at byte
// 1040 and cell 3, converted from an AMX integer cell to float, is stored at
// byte 752. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" unsigned char* objectFindObject__FPc(char* name);
static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
static void store_f32(unsigned char* address, float value) {
    union {
        float as_float;
        unsigned as_bits;
    } bits;
    bits.as_float = value;
    store_u32(address, bits.as_bits);
}
extern "C" int amxControlVehicle__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(name, cell);
    unsigned char* object = objectFindObject__FPc(name);
    if (object == 0) return 0;
    store_u32(object + 1040, static_cast<unsigned>(params[2]));
    store_f32(object + 752, static_cast<float>(params[3]));
    return 0;
}
