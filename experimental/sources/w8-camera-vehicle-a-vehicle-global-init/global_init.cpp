// Observed _GLOBAL_$I$__7VehicleG6Point3sPPci at 0x00278460, 32 bytes, and the
// file-local routine it calls (0x00277610, 652 bytes, a static initializer and
// destructor helper for the Vehicle source file). Nothing here was executed on
// the EE.
//
// The entry point calls the helper with (1, 0xffff). The helper does nothing
// unless its second argument is 0xffff and its first is 1. In that case it
// runs the ParticleDef constructor __11ParticleDefiiiiiiiifffiiiPFPv_v on six
// objects in file-local zero-initialized storage, 80 bytes apart (observed
// addresses 0x008d55b8 through 0x008d5748), then writes 0.0 to the three floats
// of a file-local 12-byte point at 0x008d5798. The constructor's argument list
// is eight ints, three floats, three ints and a function pointer, always passed
// here with a null pointer. The table below lists, in order, the ints 1..8, the
// floats as exact bit patterns, and the ints 9..11. What the values mean is not
// established here; the sizes of the objects are not either, only their stride.
extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
                                                   float x, float y, float z, int i, int j, int k, void (*callback)(void*));

namespace {
struct Spec {
    int ints[8];
    unsigned floats[3];
    int tail[3];
};

const Spec specs[6] = {
    {{80, 80, 60, 60, 80, 80, 60, 0}, {0x41f00000u, 0x42f00000u, 0x3dccccccu}, {2, 0, 4}},
    {{70, 70, 50, 60, 70, 70, 50, 0}, {0x42200000u, 0x42c80000u, 0x3dccccccu}, {2, 0, 4}},
    {{80, 80, 60, 60, 80, 80, 60, 0}, {0x42a15c28u, 0x43964666u, 0x3dccccccu}, {2, 0, 4}},
    {{70, 70, 50, 60, 70, 70, 50, 0}, {0x4272b851u, 0x43af4666u, 0x3dccccccu}, {2, 0, 4}},
    {{234, 61, 4, 128, 92, 88, 64, 22}, {0x41c80000u, 0x42480000u, 0xc0833333u}, {2, 1, 8}},
    {{79, 65, 63, 81, 50, 40, 9, 12}, {0x42480000u, 0x43160000u, 0xbfe66666u}, {2, 0, 6}},
};

alignas(8) unsigned char particle_defs[6][80] __attribute__((used));
alignas(4) unsigned char zero_point[12] __attribute__((used));

inline float float_from_bits(unsigned bits) {
    union {
        unsigned bits;
        float number;
    } converted;
    converted.bits = bits;
    return converted.number;
}

void vehicle_static_init(int initialize, int priority) {
    if (priority != 0xffff || initialize != 1) return;
    for (int n = 0; n < 6; ++n) {
        const Spec& s = specs[n];
        __11ParticleDefiiiiiiiifffiiiPFPv_v(particle_defs[n], s.ints[0], s.ints[1], s.ints[2], s.ints[3], s.ints[4], s.ints[5],
                                            s.ints[6], s.ints[7], float_from_bits(s.floats[0]), float_from_bits(s.floats[1]),
                                            float_from_bits(s.floats[2]), s.tail[0], s.tail[1], s.tail[2], nullptr);
    }
    for (int n = 0; n < 12; ++n) zero_point[n] = 0;
}
}

extern "C" void _GLOBAL_$I$__7VehicleG6Point3sPPci() { vehicle_static_init(1, 0xffff); }
