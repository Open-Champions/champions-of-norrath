// Observed: ProcessLightShafts__14VIEffectVolumef at 0x01148238, 668 bytes. Arguments: the object (this) and a float step dt. Offsets are bytes.
//  1. If the float at +92 or the float at +96 equals zero it returns 0 at once (nothing is stored).
//  2. Size__C6VIBBox(size, this+8) fills three floats (x, y, z). A zero word at +88 returns -1. Let n be the signed word at +84; when n <= 0 it
//     stores zero in the word at +32 and returns 0.
//  3. Otherwise it walks n 32-byte records starting at the pointer in the word at +88. A record's first word is a state:
//     - 0: the state becomes 1, then rand() is called seven times (r1 to r7, in this order) and the record fields are set, all as floats:
//         +4  = x * ((float)r1 - (float)r2) * 2^-32       +8  = y * 0.5
//         +12 = z * ((float)r3 - (float)r4) * 2^-32
//         +16 = ((b5 * (1/255)) * 0.39999998 + 1.5) * 0.5,  b5 = (r5 * 131) & 0xff (as an int converted to float)
//         +20 = y * ((b6 * (1/255)) * 0.19999999 + 0.79999995), b6 = (r6 * 131) & 0xff       +24 = 0
//         +28 = (k * 60 + (b7 * (1/255)) * (k * 10 + 2)) + 4, b7 = (r7 * 131) & 0xff, k = 1.0 - the float at +40
//       The y and z writes happen before the later rand calls; the calls themselves are the only external effects.
//     - 1: age = the float at +24 plus dt is stored at +24; when the float at +28 is less than or equal to age the state is set to zero.
//     - any other state: the record is skipped.
//  4. After the walk it stores zero in the word at +32 and returns 0.
// Inferred: a pool of beam-like particles that respawn when their lifetime (+28) runs out; positions are random inside the volume's box
// (x and z centred, y at half height), +16 and +20 are per-particle size/scale factors, and the float at +40 pulls the lifetime from about
// 64 down toward 4 as it rises to 1. The words at +92 and +96 are float parameters that must both be non-zero for any update.
// Unknown: the meanings of +16, +20, +40, +92 and +96; what rand() returns for the 2^-32 scale to produce a centred value (a 32-bit range
// is implied); and the record state value 1's name.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Size__C6VIBBox(float* result, const void* box);
int rand(void);

int ProcessLightShafts__14VIEffectVolumef(unsigned char* self, float dt) {
    if (*reinterpret_cast<float*>(self + 92) == 0.0f) return 0;
    if (*reinterpret_cast<float*>(self + 96) == 0.0f) return 0;
    float size[3];
    Size__C6VIBBox(size, self + 8);
    unsigned records = *reinterpret_cast<unsigned*>(self + 88);
    if (records == 0) return -1;
    int count = *reinterpret_cast<int*>(self + 84);
    if (count <= 0) {
        *reinterpret_cast<unsigned*>(self + 32) = 0;
        return 0;
    }
    unsigned char* record = NORRATH_WORD_TO_POINTER(records);
    do {
        unsigned state = *reinterpret_cast<unsigned*>(record);
        if (state == 0) {
            *reinterpret_cast<unsigned*>(record) = 1;
            int r1 = rand();
            int r2 = rand();
            float spread = static_cast<float>(r1) - static_cast<float>(r2);
            spread = size[0] * spread;
            *reinterpret_cast<float*>(record + 4) = spread * 0x1p-32f;
            *reinterpret_cast<float*>(record + 8) = size[1] * 0.5f;
            int r3 = rand();
            int r4 = rand();
            spread = static_cast<float>(r3) - static_cast<float>(r4);
            spread = size[2] * spread;
            *reinterpret_cast<float*>(record + 12) = spread * 0x1p-32f;
            int r5 = rand();
            float scale = static_cast<float>(static_cast<int>((r5 * 131u) & 0xff)) * 0x1.010100p-8f;
            *reinterpret_cast<float*>(record + 16) = (scale * 0x1.999998p-2f + 1.5f) * 0.5f;
            int r6 = rand();
            *reinterpret_cast<unsigned*>(record + 24) = 0;
            scale = static_cast<float>(static_cast<int>((r6 * 131u) & 0xff)) * 0x1.010100p-8f;
            *reinterpret_cast<float*>(record + 20) = size[1] * (scale * 0x1.999998p-3f + 0x1.999998p-1f);
            int r7 = rand();
            float k = 1.0f - *reinterpret_cast<float*>(self + 40);
            float wide = k * 10.0f;
            float span = k * 60.0f;
            scale = static_cast<float>(static_cast<int>((r7 * 131u) & 0xff)) * 0x1.010100p-8f;
            wide = wide + 2.0f;
            scale = scale * wide;
            span = span + scale;
            *reinterpret_cast<float*>(record + 28) = span + 4.0f;
        } else if (state == 1) {
            float age = *reinterpret_cast<float*>(record + 24);
            float lifetime = *reinterpret_cast<float*>(record + 28);
            age = age + dt;
            *reinterpret_cast<float*>(record + 24) = age;
            if (lifetime <= age) *reinterpret_cast<unsigned*>(record) = 0;
        }
        record += 32;
    } while (--count != 0);
    *reinterpret_cast<unsigned*>(self + 32) = 0;
    return 0;
}
}
