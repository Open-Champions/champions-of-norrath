// Observed resolveCreatureCollisions__7Vehicle at 0x00276590, 1476 bytes.
// Nothing is returned. Nothing here was executed on the EE. Field meanings are
// not established, so offsets stay numeric. V is the vehicle; n is the word at
// V+2140; objs[k] are the pointers at V+2104+4k (k < n, null entries are
// skipped); the list head is at V+2136, its nodes hold a next pointer at +0 and
// a data pointer at +12. W/F/H/B notation is word/float/halfword/unsigned byte.
//
// 1. For every non-null objs[k] = O: O.F(16..24) = V.F(16..24) + V.F(1128..1136)
//    * O.F(80), then objectUpdateInGrid__FP10GameObject(O). n is read again
//    after every iteration.
// 2. saved = objectIgnoreFlags (0x00483ac0); objectIgnoreFlags = 1. For every
//    non-null A = objs[k]: objectFindInBox__FG6Point3T0PP10GameObjecti is
//    called with the box A.F(16..24) -/+ A.F(28), a 64-entry output array and
//    the limit 64. If its first entry is null A is done. Otherwise each entry C
//    is skipped when (C.W(12) & 0x40) and C.W(1328) != 0, when C == A, when C is
//    one of the objs, when dist + 0.001f is not below A.F(28) + C.F(28) where
//    (dx, dy) = C.F(16..20) - A.F(16..20) and dist = sqrt.s(dx*dx + dy*dy), or
//    when C is already a data pointer in the V+2136 list. Otherwise
//    listOnewayLink__FP8ListHeadT0Pv(V+2136, C, C) runs, and the next steps are
//    skipped when C.W(12) & 0x200 or |dist| is not above 0.001f. Else the delta
//    (dx, dy, 0) * ((A.F(28) + C.F(28) - dist) / dist) is applied with
//    objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(C+16, delta,
//    A.F(28), 1, null) and objectUpdateInGrid__FP10GameObject(C) runs.
//    Afterwards objectIgnoreFlags = saved.
// 3. For each list node with data C: nrm = (C.F(16) - V.F(16), C.F(20) -
//    V.F(20), 0) scaled by rsqrt.s(1.0, x*x + y*y); vel =
//    getParticleVelocity__7VehicleP6Point3(V, C+16) with z set to 0.0;
//    len = sqrt.s(vel.x^2 + vel.y^2); unit = vel / len (as vel * (1.0/len));
//    dot = nrm . unit. Skipped unless dot > 0.0, then speed = len * dot must
//    exceed 4.0. If C.W(12) & 0x20: C.F(756) = (speed - 4.0) * 1966.0 /
//    (float)C.B(40) and C.H(748) = (short)iatan2__Fff(nrm.y, nrm.x). Skipped
//    when C.W(12) & 0x40 or (frameNm & 7) != 0. Otherwise with t = C.W(72), the
//    halfword at t+16 as an offset adj and the code pointer at t+20: that code
//    is called as fn(C + adj, (float)((int)((speed - 4.0) * 256.0) / C.B(40)), 0,
//    0). Which virtual method that is is not established.
// 4. listDestroy__FP8ListHead(V+2136).
extern "C" {
struct ListHead;
void objectUpdateInGrid__FP10GameObject(void* object);
int objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, unsigned char** out, int limit);
void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* second, void* value);
int objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(float* position, const float* move, float radius, int mode, float* normal);
void getParticleVelocity__7VehicleP6Point3(float* out, void* vehicle, float* point);
int iatan2__Fff(float first, float second);
void listDestroy__FP8ListHead(void* head);
extern int objectIgnoreFlags;
extern unsigned frameNm;
}

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef float __attribute__((may_alias)) AliasedFloat;
typedef unsigned char* __attribute__((may_alias)) AliasedPtr;
typedef void (*VirtualCall)(void* self, float amount, int a, int b);
typedef VirtualCall __attribute__((may_alias)) AliasedCall;

static inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}

static inline float sqrt_s(float value) {
#if defined(__mips__)
    float out;
    __asm__("sqrt.s %0, %1" : "=f"(out) : "f"(value));
    return out;
#else
    return __builtin_sqrtf(value);
#endif
}

#define WORD(p, off) (*reinterpret_cast<AliasedInt*>((p) + (off)))
#define FLOAT(p, off) (*reinterpret_cast<AliasedFloat*>((p) + (off)))
// The host contract replaces these two to model 32-bit pointers on a 64-bit host.
#ifndef LOAD_PTR
#define LOAD_PTR(p, off) (*reinterpret_cast<AliasedPtr*>((p) + (off)))
#endif
#ifndef LOAD_CALL
#define LOAD_CALL(p) (*reinterpret_cast<AliasedCall*>(p))
#endif
#define PTR(p, off) LOAD_PTR(p, off)

extern "C" void resolveCreatureCollisions__7Vehicle(unsigned char* vehicle) {
    for (int k = 0; k < WORD(vehicle, 2140); ++k) {
        unsigned char* object = PTR(vehicle, 2104 + 4 * k);
        if (object == nullptr) continue;
        const float scale = FLOAT(object, 80);
        FLOAT(object, 16) = FLOAT(vehicle, 16) + FLOAT(vehicle, 1128) * scale;
        FLOAT(object, 20) = FLOAT(vehicle, 20) + FLOAT(vehicle, 1132) * scale;
        FLOAT(object, 24) = FLOAT(vehicle, 24) + FLOAT(vehicle, 1136) * scale;
        objectUpdateInGrid__FP10GameObject(object);
    }

    const int saved = objectIgnoreFlags;
    objectIgnoreFlags = 1;
    for (int k = 0; k < WORD(vehicle, 2140); ++k) {
        unsigned char* a = PTR(vehicle, 2104 + 4 * k);
        if (a == nullptr) continue;
        float low[3], high[3];
        low[0] = FLOAT(a, 16) - FLOAT(a, 28);
        low[1] = FLOAT(a, 20) - FLOAT(a, 28);
        low[2] = FLOAT(a, 24) - FLOAT(a, 28);
        high[0] = FLOAT(a, 16) + FLOAT(a, 28);
        high[2] = FLOAT(a, 24) + FLOAT(a, 28);
        high[1] = FLOAT(a, 20) + FLOAT(a, 28);
        unsigned char* found[64];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
        if (found[0] == nullptr) continue;
        for (int j = 0; found[j] != nullptr; ++j) {
            unsigned char* c = found[j];
            if ((WORD(c, 12) & 0x40) != 0 && WORD(c, 1328) != 0) continue;
            if (c == a) continue;
            const int n = WORD(vehicle, 2140);
            bool ours = false;
            for (int m = 0; m < n; ++m) {
                if (PTR(vehicle, 2104 + 4 * m) == c) {
                    ours = true;
                    break;
                }
            }
            if (ours) continue;
            const float dx = FLOAT(c, 16) - FLOAT(a, 16);
            const float dy = FLOAT(c, 20) - FLOAT(a, 20);
            const float dist = sqrt_s(dx * dx + dy * dy);
            const float reach = FLOAT(a, 28) + FLOAT(c, 28);
            if (!(dist + 0.001f < reach)) continue;
            bool known = false;
            for (unsigned char* node = PTR(vehicle, 2136); node != nullptr; node = PTR(node, 0)) {
                if (PTR(node, 12) == c) {
                    known = true;
                    break;
                }
            }
            if (known) continue;
            listOnewayLink__FP8ListHeadT0Pv(vehicle + 2136, c, c);
            if ((WORD(c, 12) & 0x200) != 0) continue;
            if (!(0.001f < __builtin_fabsf(dist))) continue;
            const float ratio = (FLOAT(a, 28) + FLOAT(c, 28) - dist) / dist;
            float delta[3];
            delta[0] = dx * ratio;
            delta[1] = dy * ratio;
            delta[2] = 0.0f;
            objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(reinterpret_cast<float*>(c + 16), delta, FLOAT(a, 28), 1, nullptr);
            objectUpdateInGrid__FP10GameObject(c);
        }
    }
    objectIgnoreFlags = saved;

    for (unsigned char* node = PTR(vehicle, 2136); node != nullptr; node = PTR(node, 0)) {
        unsigned char* c = PTR(node, 12);
        float nrm[3];
        nrm[0] = FLOAT(c, 16) - FLOAT(vehicle, 16);
        nrm[2] = 0.0f;
        nrm[1] = FLOAT(c, 20) - FLOAT(vehicle, 20);
        const float inverse = rsqrt_s(1.0f, nrm[0] * nrm[0] + nrm[1] * nrm[1]);
        nrm[0] = nrm[0] * inverse;
        nrm[1] = nrm[1] * inverse;
        nrm[2] = nrm[2] * inverse;
        float vel[3];
        getParticleVelocity__7VehicleP6Point3(vel, vehicle, reinterpret_cast<float*>(c + 16));
        vel[2] = 0.0f;
        const float len = sqrt_s(vel[0] * vel[0] + vel[1] * vel[1]);
        const float unitScale = 1.0f / len;
        const float ux = vel[0] * unitScale;
        const float uy = vel[1] * unitScale;
        const float uz = vel[2] * unitScale;
        const float dot = nrm[0] * ux + nrm[1] * uy + nrm[2] * uz;
        if (!(0.0f < dot)) continue;
        const float speed = len * dot;
        if (!(4.0f < speed)) continue;
        int flags = WORD(c, 12);
        if ((flags & 0x20) != 0) {
            FLOAT(c, 756) = (speed - 4.0f) * 1966.0f / static_cast<float>(c[40]);
            *reinterpret_cast<AliasedShort*>(c + 748) = static_cast<short>(iatan2__Fff(nrm[1], nrm[0]));
            flags = WORD(c, 12);
        }
        if ((flags & 0x40) != 0) continue;
        if ((frameNm & 7) != 0) continue;
        unsigned char* table = PTR(c, 72) + 16;
        const int adjust = *reinterpret_cast<AliasedShort*>(table);
        const VirtualCall call = LOAD_CALL(table + 4);
        const int scaled = static_cast<int>((speed - 4.0f) * 256.0f) / c[40];
        call(c + adjust, static_cast<float>(scaled), 0, 0);
    }
    listDestroy__FP8ListHead(vehicle + 2136);
}
