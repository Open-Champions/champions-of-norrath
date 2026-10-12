// Observed _GLOBAL_$I$nmPlayers at 0x001ac4f0, 32 bytes, calling the file-local
// initializer at 0x001ab4c0 with (1, 0xffff). With those arguments the
// initializer does two things, in order.
// 1) For each of 4 `player` records (stride 23984) it walks 3 blocks of 4616
//    bytes starting at record offset 328, and in each block 64 elements of 72
//    bytes. Per element it stores -1 as halfwords at +60, +52, +44, +36, the
//    byte 0xff at +32 and +33, and zero halfwords at +16, +18, +20, +24, +26,
//    +28. Nothing else in the element is written.
// 2) It constructs six 80-byte file-local particle definitions with the
//    ParticleDef constructor (symbol names trailParticleSpark at 0x7cb550,
//    blueSpark, redSpark, fuseParticle, smokeParticle, slamParticle, 80 bytes
//    apart or more). Argument tuples are listed in the calls below. Several
//    floats come from literal words in the image; their bit patterns are given
//    in hex. What the elements and particle fields mean is not established.
extern "C" {
extern unsigned char player[];
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
                                         float x, float y, float z, int i, int j, int k, void (*callback)(void*));

static unsigned char trailParticleSpark[80];
static unsigned char blueSpark[80];
static unsigned char redSpark[80];
static unsigned char fuseParticle[80];
static unsigned char smokeParticle[80];
static unsigned char slamParticle[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void localInitialize(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        for (int record = 0; record < 4; ++record) {
            unsigned char* base = player + record * 23984 + 328;
            for (int block = 0; block < 3; ++block) {
                unsigned char* element = base + block * 4616;
                for (int n = 0; n < 64; ++n, element += 72) {
                    for (int slot = 60; slot >= 36; slot -= 8) *reinterpret_cast<short*>(element + slot) = -1;
                    element[32] = 0xff;
                    element[33] = 0xff;
                    for (int offset = 16; offset <= 20; offset += 2) {
                        *reinterpret_cast<short*>(element + offset) = 0;
                        *reinterpret_cast<short*>(element + offset + 8) = 0;
                    }
                }
            }
        }
    }
    if (initialize != 1) return;
    __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticleSpark, 200, 200, 150, 300, 200, 175, 100, 0, 2.0f, 5.0f, -20.0f,
                                        1, 2, 2, 0);
    __11ParticleDefiiiiiiiifffiiiPFPv_v(blueSpark, 57, 59, 100, 154, 56, 43, 100, -74, bits(0x4141ba5e),
                                        bits(0x4114b851), bits(0xc063e76c), 1, 1, 48, 0);
    __11ParticleDefiiiiiiiifffiiiPFPv_v(redSpark, 100, 57, 59, 154, 100, 56, 43, -74, bits(0x4141ba5e),
                                        bits(0x4114b851), bits(0xc063e76c), 1, 1, 9, 0);
    __11ParticleDefiiiiiiiifffiiiPFPv_v(fuseParticle, 255, 255, 255, 255, 0, 0, 0, 0, 8.0f, 3.0f, -5.0f, 1, 1, 0, 0);
    __11ParticleDefiiiiiiiifffiiiPFPv_v(smokeParticle, 100, 44, 0, 23, -77, -54, 0, 0, 26.0f, 21.0f,
                                        bits(0xc07851eb), 2, 0, 5, 0);
    __11ParticleDefiiiiiiiifffiiiPFPv_v(slamParticle, 228, 150, 150, 150, 255, 0, 0, 0, -8.0f, 15.0f,
                                        bits(0x80000000), 2, 2, 6, 0);
}

void _GLOBAL_$I$nmPlayers() {
    localInitialize(1, 0xffff);
}
}
