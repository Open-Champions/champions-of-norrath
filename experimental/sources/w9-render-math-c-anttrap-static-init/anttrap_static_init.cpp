// Observed _GLOBAL_$I$msg_run__7AntTrap at 0x00226578 (a 32-byte wrapper) calling the file-local
// initializer at 0x00225ef0 (248 bytes) with (1, 0xffff). With those arguments the initializer makes the two
// calls below to the ParticleDef constructor __11ParticleDefiiiiiiiifffiiiPFPv_v at 0x00180d78 with a zero
// callback. Integer tuples and float bit patterns were taken by running the image's code under a private
// interpreter with the constructor replaced by a recorder.
//   norrath_anttrap_msg_run_FireParticle_bytes      0x007d7a40  80 bytes (symbol FireParticle, file-local)
//   norrath_anttrap_msg_run_HateGateParticle_bytes  0x007d7a90  80 bytes (symbol HateGateParticle, file-local)
// The two exported names are the placeholders used by the authored Hate gate and Hate into run
// functions. What the particle fields mean is not established here. Fresh compilation is not an
// original-byte match.
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
unsigned char norrath_anttrap_msg_run_FireParticle_bytes[80];
unsigned char norrath_anttrap_msg_run_HateGateParticle_bytes[80];
}

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

__attribute__((noinline)) static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_anttrap_msg_run_FireParticle_bytes, 205, 204, 99, 5, 129, 20, 74, 4,
                bits(0x41980000u), bits(0x42000000u), bits(0x3facccccu), 1, 1, 6, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_anttrap_msg_run_HateGateParticle_bytes, 227, 204, 95, 8, 100, 56, 26, 6,
                bits(0x41200000u), bits(0x41700000u), bits(0x3f000000u), 3, 1, 11, nullptr);
    }
}

extern "C" void _GLOBAL_$I$msg_run__7AntTrap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
