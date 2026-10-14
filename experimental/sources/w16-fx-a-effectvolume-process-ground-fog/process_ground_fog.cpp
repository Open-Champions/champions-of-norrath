// Observed: ProcessGroundFog__14VIEffectVolumef at 0x01148ca0, 656 bytes. Arguments: the object (this) and a float step dt. Offsets are bytes.
//  1. If the float at +92 or the float at +96 equals zero it returns 0 at once (nothing is stored).
//  2. Size__C6VIBBox(size, this+8) fills three floats (x, y, z). A zero word at +88 returns -1. Let n be the signed word at +84; when n <= 0 it
//     stores zero in the word at +32 and returns 0.
//  3. Otherwise it walks n 36-byte records starting at the pointer in the word at +88. A record's first word is a state:
//     - 0: the state becomes 1, then rand() is called eight times (r1 to r8, in this order) and fields are set:
//         +20 = 2 * ((b1 * (1/255)) + 2.0)       +24 = y * ((b2 * (1/255)) * 0.14999999 + 0.84999996)
//         +8 = 0 (an integer zero)               +4 = x * ((float)r3 - (float)r4) * 2^-32       +12 = z * ((float)r5 - (float)r6) * 2^-32
//         +16 = r7 & 7 (a word)                  +32 = (b8 * (1/255)) * 30.0 + 10.0
//       where bN = (rN * 131) & 0xff converted to float. Then, when the word at this+32 is zero, +28 is set to zero; otherwise rand() is
//       called a ninth time (r9) and +28 = (the new +32 value) * (b9 * (1/255)).
//     - 1: age = the float at +28 plus dt is stored at +28; when the float at +32 is less than or equal to age the state is set to zero.
//     - any other state: the record is skipped.
//  4. After the walk it stores zero in the word at +32 and returns 0.
// Inferred: ground-fog puffs with a lifetime in +32 (10 to 40) and an age in +28. The word at this+32, which AllocParticles sets to 1 and this
// function clears, makes the first pass after an allocation start each puff at a random age so the fog does not all appear at once.
// Unknown: the meanings of +16, +20 and +24 (size or colour-like factors), of the floats at +92 and +96 (both must be non-zero), and the
// range of rand().
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Size__C6VIBBox(float* result, const void* box);
int rand(void);

int ProcessGroundFog__14VIEffectVolumef(unsigned char* self, float dt) {
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
            float scale = static_cast<float>(static_cast<int>((r1 * 131u) & 0xff)) * 0x1.010100p-8f;
            scale = scale + 2.0f;
            *reinterpret_cast<float*>(record + 20) = scale + scale;
            int r2 = rand();
            scale = static_cast<float>(static_cast<int>((r2 * 131u) & 0xff)) * 0x1.010100p-8f;
            scale = scale * 0x1.333332p-3f + 0x1.b33332p-1f;
            *reinterpret_cast<float*>(record + 24) = size[1] * scale;
            int r3 = rand();
            int r4 = rand();
            *reinterpret_cast<unsigned*>(record + 8) = 0;
            float spread = static_cast<float>(r3) - static_cast<float>(r4);
            spread = size[0] * spread;
            *reinterpret_cast<float*>(record + 4) = spread * 0x1p-32f;
            int r5 = rand();
            int r6 = rand();
            spread = static_cast<float>(r5) - static_cast<float>(r6);
            spread = size[2] * spread;
            *reinterpret_cast<float*>(record + 12) = spread * 0x1p-32f;
            int r7 = rand();
            *reinterpret_cast<unsigned*>(record + 16) = static_cast<unsigned>(r7) & 7;
            int r8 = rand();
            scale = static_cast<float>(static_cast<int>((r8 * 131u) & 0xff)) * 0x1.010100p-8f;
            scale = scale * 30.0f;
            *reinterpret_cast<float*>(record + 32) = scale + 10.0f;
            if (*reinterpret_cast<unsigned*>(self + 32) == 0) {
                *reinterpret_cast<unsigned*>(record + 28) = 0;
            } else {
                int r9 = rand();
                scale = static_cast<float>(static_cast<int>((r9 * 131u) & 0xff)) * 0x1.010100p-8f;
                *reinterpret_cast<float*>(record + 28) = *reinterpret_cast<float*>(record + 32) * scale;
            }
        } else if (state == 1) {
            float age = *reinterpret_cast<float*>(record + 28);
            float lifetime = *reinterpret_cast<float*>(record + 32);
            age = age + dt;
            *reinterpret_cast<float*>(record + 28) = age;
            if (lifetime <= age) *reinterpret_cast<unsigned*>(record) = 0;
        }
        record += 36;
    } while (--count != 0);
    *reinterpret_cast<unsigned*>(self + 32) = 0;
    return 0;
}
}
