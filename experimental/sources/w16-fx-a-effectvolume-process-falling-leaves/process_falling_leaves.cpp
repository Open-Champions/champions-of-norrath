// Observed: ProcessFallingLeaves__14VIEffectVolumef at 0x01147b40, 812 bytes. Arguments: the object (this) and a float step dt. Offsets are bytes.
//  1. If the float at +92 or the float at +96 equals zero it returns 0 at once (nothing is stored).
//  2. Size__C6VIBBox(size, this+8) fills three floats (x, y, z). A zero word at +88 returns -1. Let n be the signed word at +84; when n <= 0 it
//     stores zero in the word at +32 and returns 0.
//  3. Otherwise it walks n 36-byte records starting at the pointer in the word at +88. A record's first word is a state; states other than
//     0, 1 and 2 are skipped.
//     - 0: the state becomes 1 and rand() is called, with R the word at this+32 (read after the first two calls):
//         r1, r2: +12 = x * ((float)r1 - (float)r2) * 2^-32.
//         R non-zero: r3, r4 are drawn and +16 = y * ((float)r3 - (float)r4) * 2^-32; R zero: +16 = y * 0.5 and no calls are made.
//         m1, m2: +20 = z * ((float)m1 - (float)m2) * 2^-32.  m3: +28 = ((b * (1/255)) * 0.79999995) + 0.5, b = (m3 * 131) & 0xff.
//         +24 = (-y) * 0.5.  +32 = 1.0.  m4: +8 = (float)(m4 % 30), a signed remainder.  m5: the word at +4 = m5 & 1.
//     - 1: with s = dt * 30: +8 becomes (float at +8) + s, +16 becomes (float at +16) - (float at +28) * dt. When 30 <= the new +8 value it is
//       replaced by an integer zero. When the new +16 value is below the float at +24 the state becomes 2.
//     - 2: when 1.0 <= the float at +8 it grows by dt * 30 as in state 1 and resets to zero at 30 or more; otherwise the float at +32 loses
//       dt * 0.59999996. After either branch a +32 value below zero sets the state to zero.
//  4. After the walk it stores zero in the word at +32 and returns 0.
// Inferred: leaf-like particles that spawn at the top of the box (state 0 to 1), fall with a spin angle in +8 that wraps at 30 (state 1),
// then lie at the floor height held in +24 and fade a +32 alpha-like value to zero (state 2). R is the first-pass flag that AllocParticles
// sets: only then do the leaves start at a random height instead of the fixed half-height.
// Unknown: the meaning of +4 (a random bit) and of the +8 values 1.0 and 30; what rand() ranges over; whether +32 is alpha.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Size__C6VIBBox(float* result, const void* box);
int rand(void);

int ProcessFallingLeaves__14VIEffectVolumef(unsigned char* self, float dt) {
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
        if (state == 1) {
            float step = dt * 30.0f;
            float angle = *reinterpret_cast<float*>(record + 8);
            float fall = *reinterpret_cast<float*>(record + 28);
            float height = *reinterpret_cast<float*>(record + 16);
            fall = fall * dt;
            angle = angle + step;
            height = height - fall;
            *reinterpret_cast<float*>(record + 8) = angle;
            *reinterpret_cast<float*>(record + 16) = height;
            if (30.0f <= angle) *reinterpret_cast<unsigned*>(record + 8) = 0;
            if (*reinterpret_cast<float*>(record + 16) < *reinterpret_cast<float*>(record + 24)) *reinterpret_cast<unsigned*>(record) = 2;
        } else if (state == 0) {
            *reinterpret_cast<unsigned*>(record) = 1;
            int r1 = rand();
            int r2 = rand();
            float spread = static_cast<float>(r1) - static_cast<float>(r2);
            spread = size[0] * spread;
            *reinterpret_cast<float*>(record + 12) = spread * 0x1p-32f;
            if (*reinterpret_cast<unsigned*>(self + 32) != 0) {
                int r3 = rand();
                int r4 = rand();
                spread = static_cast<float>(r3) - static_cast<float>(r4);
                spread = size[1] * spread;
                *reinterpret_cast<float*>(record + 16) = spread * 0x1p-32f;
            } else {
                *reinterpret_cast<float*>(record + 16) = size[1] * 0.5f;
            }
            int m1 = rand();
            int m2 = rand();
            spread = static_cast<float>(m1) - static_cast<float>(m2);
            spread = size[2] * spread;
            *reinterpret_cast<float*>(record + 20) = spread * 0x1p-32f;
            int m3 = rand();
            float scale = static_cast<float>(static_cast<int>((m3 * 131u) & 0xff)) * 0x1.010100p-8f;
            scale = scale * 0x1.999998p-1f;
            *reinterpret_cast<float*>(record + 28) = scale + 0.5f;
            float floor = -size[1];
            *reinterpret_cast<float*>(record + 24) = floor * 0.5f;
            *reinterpret_cast<float*>(record + 32) = 1.0f;
            int m4 = rand();
            *reinterpret_cast<float*>(record + 8) = static_cast<float>(m4 % 30);
            int m5 = rand();
            *reinterpret_cast<unsigned*>(record + 4) = static_cast<unsigned>(m5) & 1;
        } else if (state == 2) {
            float angle = *reinterpret_cast<float*>(record + 8);
            if (1.0f <= angle) {
                float grown = angle + dt * 30.0f;
                *reinterpret_cast<float*>(record + 8) = grown;
                if (30.0f <= grown) *reinterpret_cast<unsigned*>(record + 8) = 0;
            } else {
                float alpha = *reinterpret_cast<float*>(record + 32);
                *reinterpret_cast<float*>(record + 32) = alpha - dt * 0x1.333332p-1f;
            }
            if (*reinterpret_cast<float*>(record + 32) < 0.0f) *reinterpret_cast<unsigned*>(record) = 0;
        }
        record += 36;
    } while (--count != 0);
    *reinterpret_cast<unsigned*>(self + 32) = 0;
    return 0;
}
}
