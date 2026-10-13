// Observed findEvadeDestination__8AntQueen at 0x002080b0, 548 bytes. The result Point3 comes back through a
// hidden first pointer (returned in V0) and the creature is the second argument. The word at object+576 is a
// node pointer (zero means none); the word at node+12 points at a block T whose floats at +16/+20 are a
// position (x, y). With a node present, up to ten tries are made; each try:
//  1. r = (short)eRand(object); c = icos(r).
//  2. e1 = eRand(object) (T re-read from the node first);  p.x = T.x + ((float)e1 * 2^-16 * 300 + 300) * c.
//  3. s = isin(r); e2 = eRand(object) (T re-read);  p.y = T.y + ((float)e2 * 2^-16 * 300 + 300) * s.
//     p.z = the float at object+24.
//  4. a = halfword(+56) - 32767 (an int, not wrapped); the probe point is (p.x + 150 * icos(a), p.y + 150 *
//     isin(a), p.z), and h = worldFindHeight(world, probe, float at object+28).
//  5. When z - 12 < h < z + 12 for z = the float at +24, the result is p (without the 150-unit shift) and the
//     function returns.
// When no node exists, or all ten tries fail, the result is a copy of the point at object+16. Which way the
// 150-unit offset points (a is the facing turned by roughly half a revolution) is inferred; what the node and
// T are is unknown. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const float* point, float radius);
extern void* world;

Point3* findEvadeDestination__8AntQueen(Point3* result, void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3 p = *reinterpret_cast<Point3*>(o + 16);
    if (word(o, 576) != 0) {
        const float scale = bits(0x37800000u);
        int tries = 0;
        do {
            const int r = static_cast<short>(eRand__10GameObject(o));
            const float c = icos__Fi(r);
            unsigned char* t = from_word(word(from_word(word(o, 576)), 12));
            const int e1 = static_cast<int>(eRand__10GameObject(o));
            p.x = real(t, 16) + (static_cast<float>(e1) * scale * 300.0f + 300.0f) * c;
            const float s = isin__Fi(r);
            t = from_word(word(from_word(word(o, 576)), 12));
            const int e2 = static_cast<int>(eRand__10GameObject(o));
            p.y = real(t, 20) + (static_cast<float>(e2) * scale * 300.0f + 300.0f) * s;
            p.z = real(o, 24);
            const int a = half(o, 56) - 32767;
            Point3 probe;
            probe.x = p.x + icos__Fi(a) * 150.0f;
            probe.y = p.y + isin__Fi(a) * 150.0f;
            probe.z = p.z;
            const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, &probe.x, real(o, 28));
            const float z = real(o, 24);
            ++tries;
            if (z - 12.0f < h && h < z + 12.0f) {
                *result = p;
                return result;
            }
        } while (tries < 10);
    }
    *result = *reinterpret_cast<Point3*>(o + 16);
    return result;
}
}
