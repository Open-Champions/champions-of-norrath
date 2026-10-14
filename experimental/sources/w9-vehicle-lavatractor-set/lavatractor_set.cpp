// Observed set__11LavaTractor at 0x00284918, 620 bytes. A fixed parameter load
// for a LavaTractor: plain stores into the vehicle object, setMassAndIBody
// after the first four (884, 888, 896, 892), then four addWheel calls (front
// and back wheel model names "wheelfrnt.vif" and "wheelbk.vif", count argument
// 1 each, then four floats) in the order front, back, back, front, then the
// remaining stores. Values are the exact float bit patterns the original uses,
// in the original store order. The meaning of the individual fields is not
// established here. Nothing here was executed on the EE.

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

extern "C" {
void setMassAndIBody__7Vehicle(unsigned char* vehicle);
void addWheel__7VehiclePciffff(unsigned char* vehicle, const char* name, int count, float a, float b, float c, float d);
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

extern "C" void set__11LavaTractor(unsigned char* vehicle) {
    put_float(vehicle, 884, 2021.0f);
    put_float(vehicle, 888, 152.0f);
    put_float(vehicle, 896, 250.0f);
    put_float(vehicle, 892, 88.0f);
    setMassAndIBody__7Vehicle(vehicle);
    put_float(vehicle, 936, 113.0f);
    put_float(vehicle, 924, 15.0f);
    put_float(vehicle, 920, 25.0f);
    put_float(vehicle, 856, 105.0f);
    put_float(vehicle, 860, 40.0f);
    put_float(vehicle, 864, 26.0f);
    put_float(vehicle, 868, 0x1.b6df66p+4f);
    put_float(vehicle, 872, 0x1.ccccccp-1f);
    put_float(vehicle, 876, -0x1.9de4d4p+0f);
    put_float(vehicle, 880, -0x1.7b404ap+2f);
    put_float(vehicle, 764, 0x1.31e85ep+1f);
    put_float(vehicle, 944, 0x1.0450b4p+4f);
    put_float(vehicle, 948, 2.0f);
    put_float(vehicle, 928, 79.0f);
    put_float(vehicle, 932, 111.0f);
    put_float(vehicle, 940, 96.0f);
    put_word(vehicle, 900, 0);
    put_word(vehicle, 904, 0);
    put_word(vehicle, 1044, 0);
    put_word(vehicle, 1000, 0);
    addWheel__7VehiclePciffff(vehicle, "wheelfrnt.vif", 1, 1.0f, 31.0f, -89.0f, -19.0f);
    addWheel__7VehiclePciffff(vehicle, "wheelbk.vif", 1, 0.0f, 47.0f, 63.0f, -19.0f);
    addWheel__7VehiclePciffff(vehicle, "wheelbk.vif", 1, 0.0f, -47.0f, 63.0f, -19.0f);
    addWheel__7VehiclePciffff(vehicle, "wheelfrnt.vif", 1, 1.0f, -31.0f, -89.0f, -19.0f);
    put_float(vehicle, 2340, 1.0f);
    put_float(vehicle, 2332, 14248.0f);
    put_float(vehicle, 916, 0x1.cad14ap-8f);
    put_float(vehicle, 2336, 2004.0f);
    put_float(vehicle, 2344, 0x1.a41a4p-2f);
    put_float(vehicle, 2348, 14737.0f);
    put_float(vehicle, 2352, 2131.0f);
    put_float(vehicle, 2356, 0x1.01c8eap+0f);
    put_address(vehicle, 2064, vehicle + 2344);
    put_address(vehicle, 2068, vehicle + 2328);
    put_float(vehicle, 908, 0x1.f9f78ep+2f);
    put_float(vehicle, 912, 0x1.440bfep-1f);
    put_float(vehicle, 2328, 0x1.a41a4p-2f);
    put_address(vehicle, 2056, vehicle + 2328);
    put_address(vehicle, 2060, vehicle + 2344);
}
