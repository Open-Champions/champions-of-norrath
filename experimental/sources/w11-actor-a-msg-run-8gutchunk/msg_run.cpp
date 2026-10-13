// Observed msg_run__8GutChunk at 0x001d4088, 1304 bytes. Argument: the object o. Nothing is returned. Names
// below: pos = floats at o+16, o+20, o+24 (px, py, pz); A = float at +452, G = float at +588, F = word at +584,
// C = word at +604, H = byte at +608. The image reads four float constants through the global pointer, none
// with a symbol: Ka = Kb = Kd = -10000.0 (0xc61c4000) at 0x004806d0, 0x004806d4 and 0x004806dc, and Kc = 0.1
// (0x3dcccccc) at 0x004806d8. All floorAdd calls take the Point3 (px, py, height), a short angle taken from an
// eRand draw, 12.0, and two ints.
//  1. msg_run__6PhProp(o) runs first.
//  2. When A > 1.0: one eRand draw; when it is divisible by 3 (unsigned) and g_BloodLevel is not zero,
//     P_AddParticle(def, copy of pos, (0,0,0), 0.0) is called with def = particleSlimeDrop when F has bit 4,
//     else particleEctoDrop when F has bit 2, else particleBloodDrop.
//  3. height = worldFindHeight(world, pos, 8.0). P2 = (px, py, height).
//  4. When (F & 9) == 1, height < pz and Ka < height: with G read now, when (G > -1.0 and A < -1.0) or
//     (G > -2.0 and A < -2.0): two eRand draws r1, r2; floorAdd(P2, (short)r1, 12.0, k + (r2 & 1), 0) with k = 4
//     for F bit 4, else 7 for bit 2, else 1.
//  5. G is read again. When G < 0.0 and A > 0.0:
//     a. When G < -1.0: soundPlaySequence(lumpFindResource("fx.lmp", "bf_water.anm"), o, 0.5).
//     b. F is read again. When F has bit 1 the object's table entry at offset 8 (adjusted by its halfword) is
//        called with 3 and the function returns (this is the way the object deletes itself; the image
//        returns without further work when o is null).
//     c. Otherwise, with C read: nothing when C is 0. Otherwise, when F has bit 8 only C-1 is stored; when it does
//        not, and height < pz and Kb < height, two eRand draws are made, the first one's short is the angle,
//        and floorAdd(P2, angle, 12.0, 4 (F bit 4) or 7 (bit 2) or 1, 0) runs, then F and C are read again;
//        then C-1 is stored at +604.
//  6. F is read again (for every path that reaches here). When F has no bit 1, H is zero, and the floats at +444
//     and +448 are both below Kc: H is set to 1; then when F has no bit 8, height <= pz and Kd < height, one
//     eRand draw gives the angle and floorAdd(P2, angle, 12.0, 3 (F bit 4) or 6 (bit 2) or 0, 1) runs.
//  7. G is set to A.
// Inferred: this is the per-frame handler of a gut chunk, from the symbol; the drops are blood, ecto or slime
// by F's low bits. Unknown: the meaning of F's bits, of A, G, C, H and the +444/+448 floats. Nothing here ran
// on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern int g_BloodLevel;
extern void* world;
extern unsigned char particleBloodDrop[], particleEctoDrop[], particleSlimeDrop[];
void msg_run__6PhProp(void* self);
unsigned eRand__10GameObject(void* self);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float f);
void floorAdd__FG6Point3sfii(Point3 position, short angle, float size, int a, int b);
void* lumpFindResource__FPcT0(char* lump, char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float volume);

void msg_run__8GutChunk(void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const float Ka = bits(0xc61c4000u), Kb = bits(0xc61c4000u), Kc = bits(0x3dcccccc), Kd = bits(0xc61c4000u);
    msg_run__6PhProp(self);
    if (1.0f < real(o, 452)) {
        if (eRand__10GameObject(self) % 3u == 0 && g_BloodLevel != 0) {
            Point3 position = *reinterpret_cast<Point3*>(o + 16);
            Point3 velocity = {0.0f, 0.0f, 0.0f};
            const unsigned f = word(o, 584);
            P_AddParticle__FP11ParticleDefP6Point3T1f((f & 4u) ? particleSlimeDrop : (f & 2u) ? particleEctoDrop : particleBloodDrop, &position, &velocity, 0.0f);
        }
    }
    Point3 pos = *reinterpret_cast<Point3*>(o + 16);
    const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, pos, 8.0f);
    Point3 ground = *reinterpret_cast<Point3*>(o + 16);
    ground.z = height;
    if ((word(o, 584) & 9u) == 1u && height < real(o, 24) && Ka < height) {
        const float g = real(o, 588);
        const float a = real(o, 452);
        if ((bits(0xbf800000u) < g && a < bits(0xbf800000u)) || (bits(0xc0000000u) < g && real(o, 452) < bits(0xc0000000u))) {
            const unsigned f = word(o, 584);
            const short angle = static_cast<short>(eRand__10GameObject(self));
            const unsigned r2 = eRand__10GameObject(self);
            floorAdd__FG6Point3sfii(ground, angle, 12.0f, ((f & 4u) ? 4 : (f & 2u) ? 7 : 1) + static_cast<int>(r2 & 1u), 0);
        }
    }
    if (real(o, 588) < 0.0f && 0.0f < real(o, 452)) {
        if (real(o, 588) < bits(0xbf800000u))
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("bf_water.anm")), self, 0.5f);
        const unsigned f = word(o, 584);
        if (f & 1u) {
            call_entry(o, 8, 3);
            return;
        }
        const unsigned c = word(o, 604);
        if (c != 0) {
            if (!(f & 8u) && height < real(o, 24) && Kb < height) {
                const short angle = static_cast<short>(eRand__10GameObject(self));
                eRand__10GameObject(self);
                floorAdd__FG6Point3sfii(ground, angle, 12.0f, (f & 4u) ? 4 : (f & 2u) ? 7 : 1, 0);
                word(o, 604) = word(o, 604) - 1u;
            } else {
                word(o, 604) = c - 1u;
            }
        }
    }
    const unsigned f = word(o, 584);
    if (!(f & 1u) && *reinterpret_cast<signed char*>(o + 608) == 0 && real(o, 444) < Kc && real(o, 448) < Kc) {
        *(o + 608) = 1;
        if (!(f & 8u) && height <= real(o, 24) && Kd < height) {
            const short angle = static_cast<short>(eRand__10GameObject(self));
            floorAdd__FG6Point3sfii(ground, angle, 12.0f, (f & 4u) ? 3 : (f & 2u) ? 6 : 0, 1);
        }
    }
    real(o, 588) = real(o, 452);
}
}
