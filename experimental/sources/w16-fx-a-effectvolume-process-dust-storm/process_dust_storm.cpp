// Observed: ProcessDustStorm__14VIEffectVolumef at 0x01149420, 988 bytes. Arguments: the object (this) and a float step dt. Offsets are bytes.
//  1. If the float at +92 or the float at +96 equals zero it returns 0 at once (nothing is stored).
//  2. Size__C6VIBBox(size, this+8) fills three floats (x, y, z). A zero word at +88 returns -1. Let n be the signed word at +84; when n <= 0 it
//     stores zero in the word at +32 and returns 0.
//  3. Otherwise it walks n 60-byte records starting at the pointer in the word at +88. A record's first word is a state; states other
//     than 0 and 1 are skipped. D is the float at this+40; bN = (rN * 131) & 0xff as a float; K = 1/255 (0x3b808080) and Q = K / 4.
//     - 0: the state becomes 1, then rand() is called twelve times, or eleven when the word at this+32 is zero (r1 to r12, in order):
//         r1: +24 = ((b1 * K + 2.0) * 2) * (D * 0.5 + 0.5)     r2: +28 = the same expression with b2
//         r3: +4 = (-x * 0.5) - (x * b3) * Q
//         r4, r5: +8 = y * ((float)r4 - (float)r5) * 2^-32     r6, r7: +12 = z * ((float)r6 - (float)r7) * 2^-32
//         r8: +52 = the float now at +8, +56 = the float now at +12, +48 = x * 0.5 + (x * b8) * Q
//         r9: +16 = +20 = r9 & 0x3c                            r10: +36 = (b10 * K) * 0.5 + 0.5
//         r11: u = ((b11 * K) * 0.70000005 + 1.0) * ((1.0 - D) + 1.0); +40 = u and +44 = 1.0 / u
//         then, with R the word at this+32: R zero stores an integer zero at +32; otherwise r12 gives +32 = (the float at +40) * (b12 * K).
//     - 1: age = the float at +32 plus dt is stored at +32. The word at +16 becomes ((int)(age * (float at +36) * ((float at +44) * 60)) + the word
//       at +20) % 60 (signed remainder). When the float at +40 is less than or equal to age the state becomes zero.
//  4. After the walk it stores zero in the word at +32 and returns 0.
// Inferred: dust motes in a drifting storm. +16 is a frame or sprite index in 0 to 59 that advances with age, +20 its random starting offset
// (a multiple of 4 up to 60), +40 a lifetime and +44 its reciprocal. The word at this+32 is the first-pass flag set by AllocParticles; when
// it is set each mote starts at a random age. +48 to +56 hold a second position taken from the spawn position, +8 and +12 plus an x spread.
// Unknown: the meaning of +4 to +12 beyond "position-like", +24 and +28 (two random size factors), +36 and the floats at +92 and +96 (both must be
// non-zero), and what rand() ranges over.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Size__C6VIBBox(float* result, const void* box);
int rand(void);

static inline float Byte(int roll) { return static_cast<float>(static_cast<int>((roll * 131u) & 0xff)); }

int ProcessDustStorm__14VIEffectVolumef(unsigned char* self, float dt) {
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
            float half = *reinterpret_cast<float*>(self + 40);
            half = half * 0.5f;
            half = half + 0.5f;
            float v = Byte(r1) * 0x1.010100p-8f;
            v = v + 2.0f;
            v = v + v;
            *reinterpret_cast<float*>(record + 24) = v * half;
            int r2 = rand();
            half = *reinterpret_cast<float*>(self + 40);
            half = half * 0.5f;
            half = half + 0.5f;
            v = Byte(r2) * 0x1.010100p-8f;
            v = v + 2.0f;
            v = v + v;
            *reinterpret_cast<float*>(record + 28) = v * half;
            int r3 = rand();
            float x = size[0];
            float lead = -x;
            lead = lead * 0.5f;
            float spread = x * Byte(r3);
            spread = spread * 0x1.010100p-10f;
            *reinterpret_cast<float*>(record + 4) = lead - spread;
            int r4 = rand();
            int r5 = rand();
            spread = static_cast<float>(r4) - static_cast<float>(r5);
            spread = size[1] * spread;
            *reinterpret_cast<float*>(record + 8) = spread * 0x1p-32f;
            int r6 = rand();
            int r7 = rand();
            spread = static_cast<float>(r6) - static_cast<float>(r7);
            spread = size[2] * spread;
            *reinterpret_cast<float*>(record + 12) = spread * 0x1p-32f;
            int r8 = rand();
            *reinterpret_cast<float*>(record + 52) = *reinterpret_cast<float*>(record + 8);
            float centre = x * 0.5f;
            *reinterpret_cast<float*>(record + 56) = *reinterpret_cast<float*>(record + 12);
            spread = x * Byte(r8);
            spread = spread * 0x1.010100p-10f;
            *reinterpret_cast<float*>(record + 48) = centre + spread;
            int r9 = rand();
            unsigned phase = static_cast<unsigned>(r9) & 0x3c;
            *reinterpret_cast<unsigned*>(record + 20) = phase;
            *reinterpret_cast<unsigned*>(record + 16) = phase;
            int r10 = rand();
            v = Byte(r10) * 0x1.010100p-8f;
            v = v * 0.5f;
            *reinterpret_cast<float*>(record + 36) = v + 0.5f;
            int r11 = rand();
            unsigned primed = *reinterpret_cast<unsigned*>(self + 32);
            float inverse = 1.0f - *reinterpret_cast<float*>(self + 40);
            inverse = inverse + 1.0f;
            v = Byte(r11) * 0x1.010100p-8f;
            v = v * 0x1.666666p-1f;
            v = v + 1.0f;
            v = v * inverse;
            float reciprocal = 1.0f / v;
            *reinterpret_cast<float*>(record + 40) = v;
            *reinterpret_cast<float*>(record + 44) = reciprocal;
            if (primed == 0) {
                *reinterpret_cast<unsigned*>(record + 32) = 0;
            } else {
                int r12 = rand();
                float lifetime = *reinterpret_cast<float*>(record + 40);
                v = Byte(r12) * 0x1.010100p-8f;
                *reinterpret_cast<float*>(record + 32) = lifetime * v;
            }
        } else if (state == 1) {
            float age = *reinterpret_cast<float*>(record + 32);
            age = age + dt;
            float phaseScale = *reinterpret_cast<float*>(record + 36);
            float rate = *reinterpret_cast<float*>(record + 44);
            int start = *reinterpret_cast<int*>(record + 20);
            rate = rate * 60.0f;
            phaseScale = age * phaseScale;
            *reinterpret_cast<float*>(record + 32) = age;
            float lifetime = *reinterpret_cast<float*>(record + 40);
            bool expired = lifetime <= age;
            phaseScale = phaseScale * rate;
            *reinterpret_cast<int*>(record + 16) = (static_cast<int>(phaseScale) + start) % 60;
            if (expired) *reinterpret_cast<unsigned*>(record) = 0;
        }
        record += 60;
    } while (--count != 0);
    *reinterpret_cast<unsigned*>(self + 32) = 0;
    return 0;
}
}
