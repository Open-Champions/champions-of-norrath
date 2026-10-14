// Observed msg_run__9WaveMaker at 0x00151658, 2960 bytes. Offsets are used for fields. W is the
// block that the word at +1892 points to (the constructor stores a "line" there): W+4 is a point
// count N, W+8 a closing flag, W+20 an array of 64-byte records (position at +0, a vector at +36,
// a float at +48). Words: +68 is the object's random seed, +1880 a frame counter, +1884 an integer
// limit, +1888 a float (the water level the constructor stores), halfwords +1876 and +1878.
// Names that start with a lower-case letter below are data owned by the original WaveMaker
// translation unit as private (static) symbols: range (a byte array), splashInfoBits (a word
// array used as a bit set), firstTime (a byte flag, initialised to 1) and seven particle
// definitions. They are imports here.
//  1. Frame counter: +1880 is incremented; t = counter * 1.7572854. (Two short delay loops of
//     nops run first; they have no effect other than time.)
//  2. For each record i < N the position p[i] = record position + record vector(+36) * t is made,
//     and its z is replaced by the float at +1888. For i > 0 the unit direction d[i] and length
//     l[i] from p[i-1] to p[i] are kept. When W+8 is nonzero one more segment from the last point to
//     the first is stored at index N. (With W+8 zero the last segment below reads stack storage that
//     was never written; the original does the same.)
//  3. Two draws of the inline random generator (seed = seed * 1664525 + 1013904223, draw = seed >> 16)
//     give a small jitter vector J = ((draw*2^-16 - 0.5) * 3, ..., 0).
//  4. For each segment i < N, with step count q = l[i+1] / record[i].float(+48), step vector =
//     d[i+1] * (q * 17) and cursor C starting at p[i], while 0 < l[i+1] the cursor advances step by
//     step; w counts steps across all segments. Each step: getWaveletMat__9WaveMakeri...(this, i,
//     travelled / l[i+1], &m) and k = 24 * range[w]. A draw taken modulo 4 (when bit w of
//     splashInfoBits is set) or 9 decides a froth particle: position C - ... along (-m[3], -m[4]),
//     jittered by two more draws, added with P_AddParticle(waveFroth, ..., J, 0), and one draw in 55
//     also calls worldPerturbWater(world, point, 394, 5). Then, unless q < 0.75 and w is odd or
//     k + 288 < t, the step counts and, when gamePositionVisible(C, 503) agrees, the collision
//     routine runs for 0 <= k - 15 - t < 1.7572854, and doWavelet (k > t) or doWaveletReflection
//     (otherwise) runs on the cursor.
//  5. Afterwards +1880 is reset to 0 when no wavelet ran and the number of visible steps is below
//     +1884; +1876 grows by +1878 (halfwords); the first time through, firstTime is cleared and seven
//     particle types are defined with P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v.
// Unknown: the meaning of the record fields and of the many float constants (read as literals). The
// 504-byte and 255-entry sizes of the two arrays are not established here. Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

struct Matrix33 { float m[9]; };

extern "C" {
extern unsigned char* world;
extern unsigned char range[] __asm__("range");
extern unsigned splashInfoBits[] __asm__("splashInfoBits");
extern signed char firstTime __asm__("firstTime");
extern unsigned char waveFroth[];
extern unsigned char waveMainParticle[];
extern unsigned char waveMainAlternate[];
extern unsigned char waveRandomFoam[];
extern unsigned char waveTrail[];
extern unsigned char waveSplashBig[];
extern unsigned char waveSplashSmall[];
void getWaveletMat__9WaveMakerifP8Matrix33(void* self, int index, float distance, Matrix33* matrix);
void doWavelet__9WaveMakerP6Point3P8Matrix33i(void* self, Point3* position, Matrix33* matrix, int count);
void doWaveletCollision__9WaveMakerP6Point3i(void* self, Point3* position, int index);
void doWaveletReflection__9WaveMakerP6Point3(void* self, Point3* position);
int gamePositionVisible__FP6Point3f(Point3* position, float radius);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float delay);
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, Point3 position, float a, float b);
void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void* definition, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y,
                                                              float z, int i, int j, int k, void (*callback)(void*));
}

namespace {
using namespace w10p;

// The inline generator of the object: the seed word at +68 advances and the draw is its top half.
unsigned draw(unsigned char* o) {
    const unsigned seed = word(o, 68) * 1664525u + 1013904223u;
    word(o, 68) = seed;
    return seed >> 16;
}
float unit(unsigned value) { return static_cast<float>(value) * bits(0x37800000u); }

void delay() {
    for (int pass = 0; pass < 2; ++pass) __asm__ volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop");
}

void define(void* d, int a, int b, int c, int dd, int e, int f, int g, int h, float x, float y, float z, int i, int j, int k) {
    P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(d, a, b, c, dd, e, f, g, h, x, y, z, i, j, k, nullptr);
}
}  // namespace

extern "C" void msg_run__9WaveMaker(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3 points[50];
    Point3 directions[52];
    float lengths[52];
    Point3 cursor;
    Point3 step;
    Point3 jitter;
    Point3 spot;
    Point3 froth;
    Matrix33 matrix;
    int waves = 0;
    int visible = 0;
    int stepsTaken = 0;
    int index = 0;

    delay();
    delay();
    unsigned char* line = from_word(word(o, 1892));
    sword(o, 1880) = sword(o, 1880) + 1;
    const float time = static_cast<float>(sword(o, 1880)) * bits(0x3fe0eeba);
    if (sword(line, 4) > 0) {
        const float level = real(o, 1888);
        for (index = 0; index < sword(line, 4); ++index) {
            unsigned char* const record = from_word(word(line, 20)) + 64 * index;
            const Point3 shift = {real(record, 36) * time, real(record, 40) * time, real(record, 44) * time};
            const Point3 here = {real(record, 0) + shift.x, real(record, 4) + shift.y, real(record, 8) + shift.z};
            points[index] = here;
            points[index].z = level;
            if (index != 0) {
                const float dx = points[index].x - points[index - 1].x;
                const float dy = points[index].y - points[index - 1].y;
                const float dz = points[index].z - points[index - 1].z;
                const float length = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
                const float scale = 1.0f / length;
                directions[index].z = dz * scale;
                directions[index].x = dx * scale;
                directions[index].y = dy * scale;
                lengths[index] = length;
            }
        }
    }
    if (word(line, 8) != 0u) {
        const float dx = points[0].x - points[index - 1].x;
        const float dy = points[0].y - points[index - 1].y;
        const float dz = points[0].z - points[index - 1].z;
        const float length = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        const float scale = 1.0f / length;
        directions[index].z = dz * scale;
        directions[index].x = dx * scale;
        directions[index].y = dy * scale;
        lengths[index] = length;
    }

    {
        const unsigned first = draw(o);
        const unsigned second = draw(o);
        jitter.x = (unit(first) - 0.5f) * 3.0f;
        jitter.y = (unit(second) - 0.5f) * 3.0f;
        jitter.z = 0.0f;
    }
    index = 0;
    if (sword(line, 4) > 0) {
        for (index = 0; index < sword(line, 4); ++index) {
            line = from_word(word(o, 1892));
            unsigned char* const record = from_word(word(line, 20)) + 64 * index;
            const float quotient = lengths[index + 1] / real(record, 48);
            cursor = points[index];
            float travelled = 0.0f;
            const float advance = quotient * 17.0f;
            step.x = directions[index + 1].x * advance;
            step.y = directions[index + 1].y * advance;
            step.z = directions[index + 1].z * advance;
            if (travelled < lengths[index + 1]) {
                do {
                    getWaveletMat__9WaveMakerifP8Matrix33(self, index, travelled / lengths[index + 1], &matrix);
                    const int base = range[waves] * 24;
                    const unsigned word32 = splashInfoBits[waves >> 5] & (1u << (waves & 31));
                    const int modulus = word32 != 0u ? 4 : 9;
                    const int roll = static_cast<int>(draw(o));
                    if (roll % modulus == 0) {
                        const unsigned second = draw(o);
                        const float back = (static_cast<float>(base) - time) - unit(second) * 24.0f;
                        const float nx = -matrix.m[3];
                        const float ny = -matrix.m[4];
                        spot.x = cursor.x + nx * back;
                        spot.y = cursor.y + ny * back;
                        spot.z = cursor.z;
                        if (gamePositionVisible__FP6Point3f(&spot, bits(0x43fb8000u)) != 0) {
                            froth = spot;
                            const unsigned third = draw(o);
                            const unsigned fourth = draw(o);
                            froth.x = froth.x + (unit(third) - 0.5f) * 12.0f;
                            froth.y = froth.y + (unit(fourth) - 0.5f) * 12.0f;
                            P_AddParticle__FP11ParticleDefP6Point3T1f(waveFroth, &froth, &jitter, 0.0f);
                            if (static_cast<int>(draw(o)) % 55 == 0)
                                worldPerturbWater__FP12_worldHeaderG6Point3ff(world, froth, bits(0x43c50000u), bits(0x40a00000u));
                        }
                    }
                    bool counts = true;
                    if (quotient < bits(0x3f400000u) && (waves & 1) != 0) counts = false;
                    if (counts && static_cast<float>(base + 288) < time) counts = false;
                    if (counts) {
                        ++stepsTaken;
                        if (gamePositionVisible__FP6Point3f(&cursor, bits(0x43fb8000u)) != 0) {
                            const float ahead = static_cast<float>(base - 15) - time;
                            if (0.0f <= ahead && ahead < bits(0x3fe0eeba)) {
                                Point3 hit = {-matrix.m[3] * 15.0f, -matrix.m[4] * 15.0f, 0.0f};
                                hit.x = hit.x + cursor.x;
                                hit.y = hit.y + cursor.y;
                                hit.z = hit.z + cursor.z;
                                doWaveletCollision__9WaveMakerP6Point3i(self, &hit, waves);
                            }
                            if (time < static_cast<float>(base)) {
                                doWavelet__9WaveMakerP6Point3P8Matrix33i(self, &cursor, &matrix, visible);
                            } else {
                                const float since = time - static_cast<float>(base);
                                const float push = since + since * 20.0f * bits(0x3c888888u);
                                const float nx = -matrix.m[3];
                                const float ny = -matrix.m[4];
                                Point3 back = {cursor.x - nx * push, cursor.y - ny * push, cursor.z};
                                doWaveletReflection__9WaveMakerP6Point3(self, &back);
                            }
                            ++visible;
                        }
                    }
                    travelled = travelled + advance;
                    cursor.x = cursor.x + step.x;
                    cursor.y = cursor.y + step.y;
                    cursor.z = cursor.z + step.z;
                    ++waves;
                } while (travelled < lengths[index + 1]);
            }
        }
    }

    if (visible == 0 && stepsTaken < sword(o, 1884)) sword(o, 1880) = 0;
    *reinterpret_cast<unsigned short*>(o + 1876) =
        static_cast<unsigned short>(*reinterpret_cast<unsigned short*>(o + 1876) + *reinterpret_cast<unsigned short*>(o + 1878));
    if (firstTime != 0) {
        firstTime = 0;
        define(waveMainParticle, 103, 118, 143, 80, 99, 116, 121, 24, bits(0x41d64236u), bits(0x42a7833du), bits(0xbf3f3882u), 1, 0, 2);
        define(waveMainAlternate, 65, 80, 93, 84, 103, 148, 111, 15, bits(0x428e0000u), bits(0x43369d57u), bits(0xc06167dbu), 2, 0, 2);
        define(waveRandomFoam, 79, 85, 74, 62, 67, 99, 89, 0, bits(0x41e06e65u), bits(0x43127b83u), bits(0xc0654fa8u), 3, 1, 2);
        define(waveTrail, 118, 127, 128, 81, 129, 132, 137, 0, bits(0x4109368fu), bits(0x40b7167eu), 0.0f, 3, 0, 2);
        define(waveSplashBig, 104, 103, 130, 93, 72, 76, 74, 0, bits(0x40e00000u), bits(0x41a00000u), bits(0xc0c00000u), 3, 1, 2);
        define(waveSplashSmall, 77, 67, 93, 85, 72, 76, 74, 0, bits(0x40e00000u), bits(0x41a00000u), bits(0xbfad0d41u), 2, 1, 2);
        define(waveFroth, 123, 124, 127, 97, 82, 84, 83, 0, bits(0x40613715u), bits(0x41a40bd3u), 0.0f, 3, 1, 5);
    }
}
