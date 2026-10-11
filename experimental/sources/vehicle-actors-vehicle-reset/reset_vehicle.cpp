// Observed Reset__7Vehiclei at 0x270f28, extent 576.
// The int selects a 64-byte record at allVehicleSpecs (0x436f68). That address
// is saved at object+2052 after refreshSettings. Twelve bytes move from
// object+1632 to object+16. Six floats at 1188 and 1200 are cleared. Word 2160
// receives frameNm (0x4844a4). Words 2152, 1028, and 1040 and floats 848 and
// 752 are cleared. The half at 1048 plus 16384 is narrowed to a signed short
// and passed to matMakeTransYaw with the updated point. That matrix becomes a
// quaternion at object+1084 and then a matrix at object+1116.
// Word 1000, captured before those stores, clears count pairs at 676 and 708
// when it is positive. Word 748 is cleared, 760 receives 1 divided by the
// float at 820, and 756 receives the float at spec+48 times the bits
// 0x3da3d70a. Bytes 108, 109, and 110 are cleared. A positive count calls
// Reset__5Wheel on each pointer at object+1052. The first wheel whose word at
// +388 is nonzero keeps its index. Later loads of object+2052 and word 1000
// are fresh. The selected wheel stores
// (spec+4 / (spec+0 / object+912)) * 0x3d888888 * (wheel+380) at object+832.
// Low V0 is -1 when no wheel is selected, otherwise that wheel pointer word.
// Records past the 64-byte spec object are outside this symbol.
extern "C" void refreshSettings__7Vehicle(void* object);
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
extern "C" void matToQuaternion__FP4QuatP8Matrix34(void* quat, void* matrix);
extern "C" void quatToMat__FP8Matrix33P4Quat(void* matrix, void* quat);
extern "C" void Reset__5Wheel(void* wheel);
extern "C" unsigned norrath_actor_view_4844a4;

namespace {
using Address = __UINTPTR_TYPE__;
__attribute__((always_inline)) inline Address address(const void* p) { return reinterpret_cast<Address>(p); }
__attribute__((always_inline)) inline unsigned address_bits(const void* p) { return static_cast<unsigned>(address(p)); }
__attribute__((always_inline)) inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
#if defined(NORRATH_VEHICLE_HOST)
struct HostRow { unsigned bits; void* full; };
HostRow host_rows[32];
#endif
__attribute__((always_inline)) inline void* pointer(Address a) {
#if defined(NORRATH_VEHICLE_HOST)
    const unsigned bits = static_cast<unsigned>(a);
    for (unsigned index = 0u; index < 32u; ++index) {
        if (host_rows[index].full != nullptr && host_rows[index].bits == bits) return host_rows[index].full;
    }
#endif
    return reinterpret_cast<void*>(a);
}
__attribute__((always_inline)) inline unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
__attribute__((always_inline)) inline void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
__attribute__((always_inline)) inline unsigned read_half(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8);
}
__attribute__((always_inline)) inline float bits_to_float(unsigned bits) {
    float value;
    unsigned char* p = reinterpret_cast<unsigned char*>(&value);
    p[0] = static_cast<unsigned char>(bits);
    p[1] = static_cast<unsigned char>(bits >> 8);
    p[2] = static_cast<unsigned char>(bits >> 16);
    p[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
__attribute__((always_inline)) inline void write_float(Address a, float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    write_word(a, unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24));
}
__attribute__((always_inline)) inline float read_float(Address a) { return bits_to_float(read_word(a)); }
__attribute__((always_inline)) inline int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value) : -1 - static_cast<int>(0xffffffffu - value);
}
struct PointBytes { unsigned x, y, z; };
__attribute__((always_inline)) inline PointBytes snapshot_point(Address a) {
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
static_assert(sizeof(PointBytes) == 12, "12-byte point");

extern "C" {
unsigned char norrath_vehicle_specs_bytes[64];
}

#if defined(NORRATH_VEHICLE_HOST)
extern "C" void norrath_vehicle_host_note(void* full) {
    const unsigned bits = static_cast<unsigned>(reinterpret_cast<Address>(full));
    for (unsigned index = 0u; index < 32u; ++index) {
        if (host_rows[index].full == nullptr || host_rows[index].bits == bits) {
            host_rows[index].bits = bits;
            host_rows[index].full = full;
            return;
        }
    }
}
#endif

extern "C" unsigned norrath_vehicle_reset(void* object, int value) {
    const Address a = address(object);
    const Address spec = address(norrath_vehicle_specs_bytes) + (static_cast<unsigned>(value) << 6);
#if defined(NORRATH_VEHICLE_HOST)
    norrath_vehicle_host_note(pointer(spec));
#endif
    refreshSettings__7Vehicle(object);
    write_word(a + 2052u, address_bits(pointer(spec)));
    write_word(a + 16u, read_word(a + 1632u));
    write_word(a + 20u, read_word(a + 1636u));
    write_word(a + 24u, read_word(a + 1640u));
    write_float(a + 1188u, 0.0f);
    write_float(a + 1192u, 0.0f);
    write_float(a + 1196u, 0.0f);
    write_float(a + 1200u, 0.0f);
    write_float(a + 1204u, 0.0f);
    write_float(a + 1208u, 0.0f);
    write_word(a + 2160u, norrath_actor_view_4844a4);
    write_word(a + 2152u, 0u);
    write_word(a + 1028u, 0u);
    write_float(a + 848u, 0.0f);
    write_float(a + 752u, 0.0f);
    write_word(a + 1040u, 0u);
    const unsigned yaw_bits = (read_half(a + 1048u) + 16384u) & 0xffffu;
    const short yaw = static_cast<short>(yaw_bits <= 0x7fffu ? static_cast<int>(yaw_bits) : static_cast<int>(yaw_bits) - 65536);
    const PointBytes position = snapshot_point(a + 16u);
    unsigned char matrix[64];
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, &position, yaw);
    matToQuaternion__FP4QuatP8Matrix34(pointer(a + 1084u), matrix);
    quatToMat__FP8Matrix33P4Quat(pointer(a + 1116u), pointer(a + 1084u));
    const int counted = signed_word(read_word(a + 1000u));
    if (counted > 0) {
        for (int index = 0; index < counted; ++index) {
            write_float(a + 676u + static_cast<unsigned>(index) * 4u, 0.0f);
            write_float(a + 708u + static_cast<unsigned>(index) * 4u, 0.0f);
        }
    }
    write_word(a + 748u, 0u);
    const float inverse = bits_to_float(0x3f800000u) / read_float(a + 820u);
    const float cleared = read_float(a + 748u);
    write_float(a + 740u, cleared);
    write_float(a + 744u, cleared);
    write_float(a + 800u, cleared);
    write_word(a + 1036u, 0u);
    write_word(a + 1032u, 0u);
    write_float(a + 760u, inverse);
    const Address loaded_spec = address(pointer(read_word(a + 2052u)));
    const float scaled = read_float(loaded_spec + 48u) * bits_to_float(0x3da3d70au);
    write_float(a + 952u, cleared);
    write_float(a + 808u, cleared);
    bytes(a)[109] = 0u;
    bytes(a)[110] = 0u;
    bytes(a)[108] = 0u;
    write_float(a + 756u, scaled);
    int selected = -1;
    if (counted > 0) {
        int index = 0;
        int limit = counted;
        while (index < limit) {
            const unsigned slot = 1052u + static_cast<unsigned>(index) * 4u;
            void* wheel = pointer(read_word(a + slot));
            Reset__5Wheel(wheel);
            const unsigned marker = read_word(address(wheel) + 388u);
            if (marker != 0u && selected < 0) selected = index;
            ++index;
            limit = signed_word(read_word(a + 1000u));
        }
    }
    if (selected < 0) return 0xffffffffu;
    const Address chosen_spec = address(pointer(read_word(a + 2052u)));
    const float spec_ratio = read_float(chosen_spec) / read_float(a + 912u);
    float value_ratio = read_float(chosen_spec + 4u) / spec_ratio;
    const unsigned chosen_slot = 1052u + static_cast<unsigned>(selected) * 4u;
    void* chosen = pointer(read_word(a + chosen_slot));
    value_ratio = value_ratio * bits_to_float(0x3d888888u) * read_float(address(chosen) + 380u);
    write_float(a + 832u, value_ratio);
    return read_word(a + chosen_slot);
}
