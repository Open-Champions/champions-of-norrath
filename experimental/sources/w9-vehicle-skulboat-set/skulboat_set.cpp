// Observed set__8SkulBoat at 0x002bf640, 328 bytes. Same store layout as
// set__4Boat with different float values; plain stores into the vehicle object,
// setMassAndIBody__7Vehicle called once after the first four (884, 896, 892,
// 888). Values are the exact float bit patterns, in the original store order.
// The meaning of the individual fields is not established here. Nothing here
// was executed on the EE.

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

extern "C" {
void setMassAndIBody__7Vehicle(unsigned char* vehicle);
}

static inline void put_float(unsigned char* base, int offset, float value) {
    *reinterpret_cast<AliasedFloat*>(base + offset) = value;
}
static inline void put_word(unsigned char* base, int offset, int value) {
    *reinterpret_cast<AliasedInt*>(base + offset) = value;
}
static inline void put_address(unsigned char* base, int offset, unsigned char* value) {
    *reinterpret_cast<AliasedPointer*>(base + offset) = value;
}

extern "C" void set__8SkulBoat(unsigned char* vehicle) {
    put_float(vehicle, 884, 10000.0f);
    put_float(vehicle, 896, 250.0f);
    put_float(vehicle, 892, 120.0f);
    put_float(vehicle, 888, 120.0f);
    setMassAndIBody__7Vehicle(vehicle);
    put_float(vehicle, 940, 50.0f);
    put_float(vehicle, 924, 15.0f);
    put_float(vehicle, 920, 25.0f);
    put_word(vehicle, 1044, 1);
    put_float(vehicle, 856, 150.0f);
    put_float(vehicle, 864, 30.0f);
    put_float(vehicle, 868, 45.0f);
    put_float(vehicle, 872, 0x1.ccccccp-1f);
    put_float(vehicle, 876, -0x1.999998p-5f);
    put_float(vehicle, 880, -0x1.999998p-4f);
    put_float(vehicle, 764, 0x1.199998p+0f);
    put_float(vehicle, 944, 40.0f);
    put_float(vehicle, 948, 2.0f);
    put_float(vehicle, 2344, 0x1.d89d88p-2f);
    put_float(vehicle, 2348, 90000.0f);
    put_float(vehicle, 2352, 5000.0f);
    put_address(vehicle, 2064, vehicle + 2344);
    put_address(vehicle, 2068, vehicle + 2328);
    put_float(vehicle, 928, 50.0f);
    put_float(vehicle, 932, 50.0f);
    put_float(vehicle, 936, 50.0f);
    put_float(vehicle, 860, 45.0f);
    put_float(vehicle, 2328, 0x1.d89d88p-2f);
    put_float(vehicle, 2332, 90000.0f);
    put_float(vehicle, 2336, 5000.0f);
    put_float(vehicle, 2340, 1.0f);
    put_float(vehicle, 2356, 1.0f);
    put_address(vehicle, 2056, vehicle + 2328);
    put_address(vehicle, 2060, vehicle + 2344);
    put_float(vehicle, 908, 1.0f);
    put_word(vehicle, 900, 0);
    put_word(vehicle, 904, 0);
    put_word(vehicle, 1000, 0);
    put_float(vehicle, 912, 1.0f);
    put_float(vehicle, 916, 0x1.999998p-5f);
}
