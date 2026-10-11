// amxEntityRideVehicle is the 284-byte native at 0x00138e00. Cell 1 names
// the rider and cell 2 names the vehicle. Cells 3 through 5 become a float
// point. The virtual entry 152 bytes into the vehicle table at byte 72 is a
// signed 16-bit this-adjustment and a function word. That function receives
// the adjusted vehicle, the point, a three-float output, and a halfword
// output. Twelve bytes of the float output are stored at rider byte 16. The
// halfword output plus the unsigned halfword at cell-byte 24 is stored at
// rider byte 56. objectUpdateInGrid then receives the rider. The native
// returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" unsigned char* objectFindObject__FPc(char* name);
extern "C" void objectUpdateInGrid__FP10GameObject(unsigned char* object);
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
static unsigned short load_u16(const unsigned char* address) {
    return static_cast<unsigned short>(static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8));
}
static void store_u16(unsigned char* address, unsigned short value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
}
static unsigned char* pointer_at(const unsigned char* address) {
    unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    if (word == 0u) return 0;
    extern unsigned char* norrath_native_host_base;
    return norrath_native_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
static void call_ride(const unsigned char* entry, unsigned char* vehicle, float* in_point, float* out_point,
                      unsigned short* out_half) {
    int adjust = static_cast<short>(load_u16(entry));
    unsigned word = load_u32(entry + 4);
    unsigned char* self = vehicle + adjust;
#if defined(NORRATH_HOST_CONTRACT)
    extern void (*norrath_native_host_ride[])(unsigned char*, float*, float*, unsigned short*);
    norrath_native_host_ride[word](self, in_point, out_point, out_half);
#else
    reinterpret_cast<void (*)(unsigned char*, float*, float*, unsigned short*)>(static_cast<__UINTPTR_TYPE__>(word))(
        self, in_point, out_point, out_half);
#endif
}
extern "C" int amxEntityRideVehicle__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char rider_name[160];
    char vehicle_name[160];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(rider_name, cell);
    amx_GetAddr(amx, params[2], &cell);
    amx_GetString(vehicle_name, cell);
    unsigned char* vehicle = objectFindObject__FPc(vehicle_name);
    unsigned char* rider = objectFindObject__FPc(rider_name);
    float in_point[3];
    float out_point[3];
    unsigned short out_half = 0;
    in_point[0] = static_cast<float>(params[3]);
    in_point[1] = static_cast<float>(params[4]);
    in_point[2] = static_cast<float>(params[5]);
    call_ride(pointer_at(vehicle + 72) + 152, vehicle, in_point, out_point, &out_half);
    for (int index = 0; index < 12; ++index)
        rider[16 + index] = reinterpret_cast<unsigned char*>(out_point)[index];
    unsigned sum = static_cast<unsigned>(out_half) + load_u16(reinterpret_cast<unsigned char*>(params) + 24);
    store_u16(rider + 56, static_cast<unsigned short>(sum));
    objectUpdateInGrid__FP10GameObject(rider);
    return 0;
}
