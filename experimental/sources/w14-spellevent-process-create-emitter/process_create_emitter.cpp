// Observed: ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb at 0x0117ec78 is 1716
// bytes in the pinned SLUS-20565 image. Arguments: (this, node, flag). Always returns 0. ev = the word
// at node+116 (the script record); emitter = the word at node+120. On entry it zeroes two stack float
// triples (v0, v1) and a 4x4 float matrix m0 (destructor _$_10VIMatrix44(m0, 2) runs on every path).
// Nothing is done at all when the int at this+24 is -1 (except that destructor).
//  Part A, only when flag is exactly 1. src = the int at this+24; when bit 0x10000 of the int at ev+12
//   is set, src = the int at (the word at this+16)+8; when bit 0x20000 is set, src = the int at
//   (the word at this+16)+12 (this one wins). With src = -1 the whole function jumps to part C. With
//   sprite = GetActorSpritePtr__C7VIScenei(the word at this+20, src) non-null: matrix m1 (sixteen zeros)
//   and a matrix33 object (ctor __10VIMatrix33, dtor _$_10VIMatrix33(p, 2)); the transform of src goes
//   into m1 exactly as in ProcessCreateSound (CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7
//   VISceneR10VIMatrix44 when the sprite's int at +52 is 5 and the halfword at ev+12 is not 0xffff,
//   else GetActorTransform__7VISceneiR10VIMatrix44). mode = (the int at ev+12 >> 20) & 3.
//   mode 0: v0 = m1[12..14].
//   mode 1, 2, 3: t = sin(the float at node+160) * 0.5 + 0.5 (the double sine goes through fptodp,
//   sin and dptofp); a = the float at node+156; lerp = ev[20] + (ev[24] - ev[20]) * t (floats at ev+20,
//   ev+24); and RotateHPR__10VIMatrix44RC7VIVect3(m0, angles) followed by the local vector
//   p = (0, lerp, ev[36]) (mode 2, angles (a, 0, 0)), (lerp, ev[36], 0) (mode 1, angles (0, a, 0)) or
//   (ev[36], 0, lerp) (mode 3, angles (0, 0, a)); ev[36] is the float at ev+36. Then
//   Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, p, v1); the float at node+156 grows by (ev[32] + ev[32]) *
//   DeltaTime__C16VIParticleSystem(scene + 1352) and the float at node+160 by ev[28] times a second
//   DeltaTime; each is wrapped once into [-2pi, 2pi] (above 2pi: minus 2pi; below -2pi: plus 2pi,
//   2pi = bits 0x40c90fda, written as the hex float 0x1.921fb4p+2f because the compiler truncates decimal float literals); and Mul(m1, v1, v0).
//   Then for every mode: the 3x3 of m1 (floats 0 1 2 4 5 6 8 9 10) is copied into the matrix33 object,
//   SetLocation__17VIParticleEmitterRC7VIVect3(emitter, v0), SetDirection__17VIParticleEmitterRC10VI
//   Matrix33(emitter, the matrix33), Update__17VIParticleEmitter(emitter), then the two destructors.
//   Without a sprite part A is skipped.
//  Part B, when this+24 is not -1 and (flag is not 1, or part A found src not -1): a = the int at
//   node+132. When it is -1: ClearAttractor__17VIParticleEmitter(emitter). Otherwise src is chosen from
//   a as in part A (bit 0x10000: effect driver int +8, bit 0x20000: +12; no -1 test), sprite =
//   GetActorSpritePtr(scene, src); when non-null: a zeroed matrix m2, the sprite transform as above
//   (halfword at node+132 as the slot), then SetAttractor__17VIParticleEmitterRC7VIVect3f(emitter,
//   m2[12..14], the float at node+136) and _$_10VIMatrix44(m2, 2).
//  Part C: when flag is 1 and the int at ev+16 is non-zero and it is below the signed low 32 bits of
//   (GetTime__7VIScene(scene) - the 64-bit value at node+176): Remove__7VILNode(node) and
//   AddTail__7VILListP7VILNode(this+88, node) (the node moves to the second list).
// Inferred: the per-frame step of a particle-emitter event (record type 3): it places and orients the
// emitter at the caster, the target or a slot, optionally swinging it around by an oscillating angle
// pair (modes 1-3 turn the swing offset into the world frame), re-aims its attractor and retires the
// event to the inactive list once its duration (ev+16) is over (the emitter then lives on until its
// particles are gone: ProcessInactiveEvents).
// Unknown: what the 2-bit mode at ev+12 bits 20-21 means, the meanings of ev+28/+32/+36 and node+156/
// +160 beyond oscillation angles and their rates, and the sine argument unit. Nothing here ran on the
// EE.
struct Vec3 {
    float x, y, z;
};
extern "C" {
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
void __10VIMatrix33(void* matrix);
void _$_10VIMatrix33(void* matrix, int flags);
void _$_10VIMatrix44(void* matrix, int flags);
void CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(void* sprite, int slot, int actor, void* scene, void* matrix);
void GetActorTransform__7VISceneiR10VIMatrix44(void* scene, int actor, void* matrix);
unsigned long long fptodp(float value);
unsigned long long sin(unsigned long long value);
float dptofp(unsigned long long value);
void RotateHPR__10VIMatrix44RC7VIVect3(void* matrix, const Vec3* angles);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const Vec3* in, Vec3* out);
float DeltaTime__C16VIParticleSystem(void* system);
void SetLocation__17VIParticleEmitterRC7VIVect3(void* emitter, const Vec3* position);
void SetDirection__17VIParticleEmitterRC10VIMatrix33(void* emitter, const void* matrix);
void Update__17VIParticleEmitter(void* emitter);
void SetAttractor__17VIParticleEmitterRC7VIVect3f(void* emitter, const Vec3* position, float strength);
void ClearAttractor__17VIParticleEmitter(void* emitter);
long long GetTime__7VIScene(void* scene);
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);

static inline void zeroMatrix(float* m) {
    for (int i = 0; i < 16; ++i) {
        m[i] = 0.0f;
    }
}

// the sprite transform of actor src into m (shared by part A and part B)
static inline void fetchTransform(unsigned char* self, unsigned char* sprite, int src, unsigned slotWord, float* m) {
    if (*reinterpret_cast<int*>(sprite + 52) == 5 && (slotWord & 0xffff) != 0xffff) {
        CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(sprite, slotWord & 0xffff, src, *reinterpret_cast<void**>(self + 20), m);
    } else {
        GetActorTransform__7VISceneiR10VIMatrix44(*reinterpret_cast<void**>(self + 20), src, m);
    }
}

static inline void attractorPart(unsigned char* self, unsigned char* node) {
    unsigned char* emitterPtr = *reinterpret_cast<unsigned char**>(node + 120);
    int a = *reinterpret_cast<int*>(node + 132);
    if (a == -1) {
        ClearAttractor__17VIParticleEmitter(emitterPtr);
        return;
    }
    int src;
    if ((a & 0x10000) != 0) {
        src = *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(self + 16) + 8);
    } else {
        src = *reinterpret_cast<int*>(self + 24);
    }
    if ((a & 0x20000) != 0) {
        src = *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(self + 16) + 12);
    }
    unsigned char* sprite = static_cast<unsigned char*>(GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), src));
    if (sprite == 0) {
        return;
    }
    alignas(16) float m2[16];
    zeroMatrix(m2);
    unsigned slotWord = *reinterpret_cast<unsigned short*>(node + 132);
    fetchTransform(self, sprite, src, slotWord, m2);
    Vec3 position;
    position.x = m2[12];
    position.y = m2[13];
    position.z = m2[14];
    SetAttractor__17VIParticleEmitterRC7VIVect3f(emitterPtr, &position, *reinterpret_cast<float*>(node + 136));
    _$_10VIMatrix44(m2, 2);
}

int ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb(unsigned char* self, unsigned char* node, int flag) {
    Vec3 v0 = {0.0f, 0.0f, 0.0f};
    Vec3 v1 = {0.0f, 0.0f, 0.0f};
    alignas(16) float m0[16];
    zeroMatrix(m0);
    unsigned char* ev = *reinterpret_cast<unsigned char**>(node + 116);
    int actor = *reinterpret_cast<int*>(self + 24);
    if (actor != -1) {
        bool attractor = true;
        if (flag == 1) {
            int flags = *reinterpret_cast<int*>(ev + 12);
            int src = actor;
            if ((flags & 0x10000) != 0) {
                src = *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(self + 16) + 8);
            }
            if ((flags & 0x20000) != 0) {
                src = *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(self + 16) + 12);
            }
            if (src == -1) {
                attractor = false;
            } else {
                unsigned char* sprite = static_cast<unsigned char*>(GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), src));
                if (sprite != 0) {
                    alignas(16) float m1[16];
                    float rotation[9];
                    zeroMatrix(m1);
                    __10VIMatrix33(rotation);
                    fetchTransform(self, sprite, src, *reinterpret_cast<unsigned*>(ev + 12), m1);
                    int mode = (*reinterpret_cast<int*>(ev + 12) >> 20) & 3;
                    if (mode == 0) {
                        v0.x = m1[12];
                        v0.y = m1[13];
                        v0.z = m1[14];
                    } else {
                        Vec3 p = {0.0f, 0.0f, 0.0f};
                        float t = dptofp(sin(fptodp(*reinterpret_cast<float*>(node + 160)))) * 0.5f + 0.5f;
                        float a = *reinterpret_cast<float*>(node + 156);
                        float low = *reinterpret_cast<float*>(ev + 20);
                        float lerp = low + (*reinterpret_cast<float*>(ev + 24) - low) * t;
                        float fixedValue = *reinterpret_cast<float*>(ev + 36);
                        if (mode == 2) {
                            Vec3 angles = {a, 0.0f, 0.0f};
                            RotateHPR__10VIMatrix44RC7VIVect3(m0, &angles);
                            p.x = 0.0f;
                            p.y = lerp;
                            p.z = fixedValue;
                        } else if (mode == 1) {
                            Vec3 angles = {0.0f, a, 0.0f};
                            RotateHPR__10VIMatrix44RC7VIVect3(m0, &angles);
                            p.x = lerp;
                            p.y = fixedValue;
                            p.z = 0.0f;
                        } else {
                            Vec3 angles = {0.0f, 0.0f, a};
                            RotateHPR__10VIMatrix44RC7VIVect3(m0, &angles);
                            p.x = fixedValue;
                            p.y = 0.0f;
                            p.z = lerp;
                        }
                        Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &p, &v1);
                        float* angleA = reinterpret_cast<float*>(node + 156);
                        float* angleB = reinterpret_cast<float*>(node + 160);
                        *angleA = *angleA + (*reinterpret_cast<float*>(ev + 32) + *reinterpret_cast<float*>(ev + 32)) *
                                                DeltaTime__C16VIParticleSystem(*reinterpret_cast<unsigned char**>(self + 20) + 1352);
                        *angleB = *angleB + *reinterpret_cast<float*>(ev + 28) *
                                                DeltaTime__C16VIParticleSystem(*reinterpret_cast<unsigned char**>(self + 20) + 1352);
                        const float twoPi = 0x1.921fb4p+2f;
                        float wrapA = *angleA;
                        if (wrapA > twoPi) {
                            *angleA = wrapA - twoPi;
                        } else if (wrapA < -twoPi) {
                            *angleA = wrapA + twoPi;
                        }
                        float wrapB = *angleB;
                        if (wrapB > twoPi) {
                            *angleB = wrapB - twoPi;
                        } else if (wrapB < -twoPi) {
                            *angleB = wrapB + twoPi;
                        }
                        Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, &v1, &v0);
                    }
                    rotation[0] = m1[0];
                    rotation[1] = m1[1];
                    rotation[2] = m1[2];
                    rotation[3] = m1[4];
                    rotation[4] = m1[5];
                    rotation[5] = m1[6];
                    rotation[6] = m1[8];
                    rotation[7] = m1[9];
                    rotation[8] = m1[10];
                    SetLocation__17VIParticleEmitterRC7VIVect3(*reinterpret_cast<void**>(node + 120), &v0);
                    SetDirection__17VIParticleEmitterRC10VIMatrix33(*reinterpret_cast<void**>(node + 120), rotation);
                    Update__17VIParticleEmitter(*reinterpret_cast<void**>(node + 120));
                    _$_10VIMatrix33(rotation, 2);
                    _$_10VIMatrix44(m1, 2);
                }
            }
        }
        if (attractor) {
            attractorPart(self, node);
        }
    }
    if (flag == 1 && *reinterpret_cast<int*>(ev + 16) != 0) {
        long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
        if (*reinterpret_cast<int*>(ev + 16) < static_cast<int>(now - *reinterpret_cast<long long*>(node + 176))) {
            Remove__7VILNode(node);
            AddTail__7VILListP7VILNode(self + 88, node);
        }
    }
    _$_10VIMatrix44(m0, 2);
    return 0;
}
}
