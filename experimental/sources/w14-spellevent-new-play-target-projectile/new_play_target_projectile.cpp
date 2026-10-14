// Observed: NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent at 0x0117dc50 is 2452 bytes in
// the pinned SLUS-20565 image. Arguments: (this, record). Returns -1 in one case, 0 otherwise. It starts
// with two zeroed 4x4 float matrices m0 and m1 and three zero float triples w1, w2, vv (the destructor
// _$_10VIMatrix44(m1, 2) then (m0, 2) runs on exit). e = the word at this+16 (the effect driver, read
// again for every loop test); scene = the word at this+20; id = the int at this+24. Loop i from 0 while
// i < (the int at e+16) + 1:
//   1. d = 0. flags = the int at record+4. When bit 0 is set and i == 0: d = AddSpellEventDriver__19
//      VISpellEffectDriverP16VISpellEventListi(e, (the word at e) + 24, the int at e+12) and flags is read
//      again. When bit 1 of flags is set and i != 0: d = AddSpellEventDriver(e, (the word at e) + 36, the
//      int at e + 16 + 4 * i).
//   2. Nothing more for this i when id is -1 or d is zero. Otherwise: a = __builtin_new(184); when
//      non-null __18VISpellActiveEventP12VISpellEventP7VIScene(a, record, scene); the 64-bit
//      GetTime__7VIScene(scene) is stored at a+176 (also when a is null); sprite = GetActorSpritePtr__C7VI
//      Scenei(scene, id).
//   3. When sprite is non-null, the int at sprite+52 is 5 and bit 12 of the int at record+12 is set:
//      sp2 = GetActorSpritePtr(scene, id) (null: clean up and return -1); k = the int at sp2+4372, or
//      the int at sp2+4384 when that is -1; when k is not negative, actor = CreateActor__7VISceneRC7VIVect3T1
//      fi(scene, {0, -8000, 0}, {0, 0, 0}, 1.0f, k) is stored at a+140 and SetActorCollideable__7VISceneii
//      (scene, actor, 0) is called.
//   4. When the int at record+8 is not negative: e2 = __builtin_new(992) (constructed with
//      __17VIParticleEmitter when non-null) is stored at a+120; Create__17VIParticleEmitterP16VIParticle
//      SystemP20VIParticleDefinition(the word at a+120, scene + 1352, ParticleDefinition__16VIParticle
//      Systemi(scene + 1352, that int)).
//   5. n = (the int at record+12 >> 8) & 15: n == 1 stores at a+156 zero when the low byte of the int at
//      record+12 is 3, else (float)rand() times the float constant 0x1.8836p-13 (bits 0x39441b00), and
//      stores zero at a+160; n == 2 stores 1.0f at a+156; other n store nothing. (The image reads the
//      int at sprite+52 afterwards even when sprite is null, i.e. address 52.)
//   6. When the int at sprite+52 is not 5: loc = GetActorLocation__C7VIScenei(scene, id); the three
//      floats of loc go to a+20; zero to the floats a+92..100. Otherwise by the low byte b of the int at
//      record+12: 0 or 1: GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44(this, id,
//      b, m0) and the position m0[12..14] is stored at a+20, zero at a+92..100; 2: the midpoint (sum
//      times 0.5f componentwise) of the positions of nodes 0 and 1 goes to a+20, zero at a+92..100;
//      3: the same midpoint at a+20, and then m0 = GetActorTransform__7VISceneiR10VIMatrix44(scene, id),
//      m1 = Transpose__10VIMatrix44RC10VIMatrix44(m1, m0) with its rows 3, 7 and 11 turned into the
//      negated dot products with columns 0, 1, 2 (rows then zeroed), Mul__C10VIMatrix44RC7VIVect3R7VIVect3
//      (m1, node0 position, a+44), Mul(m1, node1 position, a+56), Mul(m1, a+20, vv), and vv subtracted
//      from both a+44 and a+56; then, when bit 12 of the int at record+12 is set, the step-3 actor is
//      created again but stored at a+144 (same -1 exit when the sprite is null); then the step-4
//      emitter again but stored at a+124; then a+44 is copied to a+92 and a+56 to a+104 (three floats
//      each); other values of b store nothing.
//   7. Always last: the three floats at a+20 are copied to a+32, and d+36 receives a.
// Inferred: record type 8 of the event table (projectile aimed at the target): per target a child event
// driver d gets an active event a at d+36 (the slot Process tests before ProcessProjectileEvent); a+20
// is the start position, a+32 the current one, a+92/+104 the relative offsets for the two projectile
// end points when mode 3 is used, a+156/+160 two random swing values, and the actors at a+140/+144
// are hidden helper actors (placed at y = -8000, made non-collideable).
// Unknown: the swing meaning, the 4372 and 4384 sprite fields, the node numbers, the constant's unit,
// the null-sprite read. Nothing here ran on the EE.
struct Vec3 {
    float x, y, z;
};
extern "C" {
void* AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(void* effect, void* list, int id);
void* __builtin_new(unsigned size);
void __18VISpellActiveEventP12VISpellEventP7VIScene(void* self, void* event, void* scene);
long long GetTime__7VIScene(void* scene);
unsigned char* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
int CreateActor__7VISceneRC7VIVect3T1fi(void* scene, const Vec3* position, const Vec3* velocity, float amount, int kind);
void SetActorCollideable__7VISceneii(void* scene, int actor, int flag);
void __17VIParticleEmitter(void* emitter);
void* ParticleDefinition__16VIParticleSystemi(void* system, int index);
void Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(void* emitter, void* system, void* definition);
int rand();
const Vec3* GetActorLocation__C7VIScenei(void* scene, int actor);
void GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44(void* driver, int actor, int node, void* matrix);
void GetActorTransform__7VISceneiR10VIMatrix44(void* scene, int actor, void* matrix);
void Transpose__10VIMatrix44RC10VIMatrix44(void* out, const void* in);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const Vec3* in, Vec3* out);
void _$_10VIMatrix44(void* matrix, int flags);

static inline Vec3 nodePosition(unsigned char* self, int id, int node, float* m) {
    GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44(self, id, node, m);
    Vec3 v;
    v.x = m[12];
    v.y = m[13];
    v.z = m[14];
    return v;
}

static inline void storeVec(unsigned char* where, const Vec3& v) {
    reinterpret_cast<float*>(where)[0] = v.x;
    reinterpret_cast<float*>(where)[1] = v.y;
    reinterpret_cast<float*>(where)[2] = v.z;
}

static inline void zeroVec(unsigned char* where) {
    reinterpret_cast<float*>(where)[0] = 0.0f;
    reinterpret_cast<float*>(where)[1] = 0.0f;
    reinterpret_cast<float*>(where)[2] = 0.0f;
}

// creates the hidden helper actor of step 3 for sprite sp2 into *store; skipped when k is negative
static inline void helperActor(unsigned char* self, unsigned char* sp2, int* store) {
    int k = *reinterpret_cast<int*>(sp2 + 4372);
    if (k == -1) {
        k = *reinterpret_cast<int*>(sp2 + 4384);
    }
    if (k < 0) {
        return;
    }
    Vec3 position = {0.0f, -8000.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};
    *store = CreateActor__7VISceneRC7VIVect3T1fi(*reinterpret_cast<void**>(self + 20), &position, &velocity, 1.0f, k);
    SetActorCollideable__7VISceneii(*reinterpret_cast<void**>(self + 20), *store, 0);
}

static inline void makeEmitter(unsigned char* self, unsigned char* record, unsigned char* a, int slot) {
    void* emitter = __builtin_new(992);
    if (emitter != 0) {
        __17VIParticleEmitter(emitter);
    }
    unsigned char* system = *reinterpret_cast<unsigned char**>(self + 20) + 1352;
    *reinterpret_cast<void**>(a + slot) = emitter;
    void* definition = ParticleDefinition__16VIParticleSystemi(system, *reinterpret_cast<int*>(record + 8));
    Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(*reinterpret_cast<void**>(a + slot), system, definition);
}

int NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    alignas(16) float m0[16];
    alignas(16) float m1[16];
    for (int i = 0; i < 16; ++i) {
        m0[i] = 0.0f;
    }
    for (int i = 0; i < 16; ++i) {
        m1[i] = 0.0f;
    }
    Vec3 w1 = {0.0f, 0.0f, 0.0f};
    Vec3 w2 = {0.0f, 0.0f, 0.0f};
    Vec3 vv = {0.0f, 0.0f, 0.0f};
    for (int i = 0; i < *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(self + 16) + 16) + 1; ++i) {
        unsigned char* effect = *reinterpret_cast<unsigned char**>(self + 16);
        int flags = *reinterpret_cast<int*>(record + 4);
        unsigned char* d = 0;
        if ((flags & 1) != 0 && i == 0) {
            d = static_cast<unsigned char*>(AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(
                effect, *reinterpret_cast<unsigned char**>(effect) + 24, *reinterpret_cast<int*>(effect + 12)));
            flags = *reinterpret_cast<int*>(record + 4);
        }
        if ((flags & 2) != 0 && i != 0) {
            effect = *reinterpret_cast<unsigned char**>(self + 16);
            d = static_cast<unsigned char*>(AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(
                effect, *reinterpret_cast<unsigned char**>(effect) + 36, *reinterpret_cast<int*>(effect + 16 + 4 * i)));
        }
        int id = *reinterpret_cast<int*>(self + 24);
        if (id == -1 || d == 0) {
            continue;
        }
        unsigned char* a = static_cast<unsigned char*>(__builtin_new(184));
        if (a != 0) {
            __18VISpellActiveEventP12VISpellEventP7VIScene(a, record, *reinterpret_cast<void**>(self + 20));
        }
        *reinterpret_cast<long long*>(a + 176) = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
        unsigned char* sprite = GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(self + 24));
        if (sprite != 0 && *reinterpret_cast<int*>(sprite + 52) == 5 && ((*reinterpret_cast<int*>(record + 12) >> 12) & 1) != 0) {
            unsigned char* sp2 = GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(self + 24));
            if (sp2 == 0) {
                _$_10VIMatrix44(m1, 2);
                _$_10VIMatrix44(m0, 2);
                return -1;
            }
            helperActor(self, sp2, reinterpret_cast<int*>(a + 140));
        }
        if (*reinterpret_cast<int*>(record + 8) >= 0) {
            makeEmitter(self, record, a, 120);
        }
        int mode = (*reinterpret_cast<int*>(record + 12) >> 8) & 15;
        if (mode == 1) {
            if ((*reinterpret_cast<unsigned*>(record + 12) & 0xff) == 3) {
                *reinterpret_cast<unsigned*>(a + 156) = 0;
            } else {
                *reinterpret_cast<float*>(a + 156) = static_cast<float>(rand()) * 0x1.8836p-13f;
            }
            *reinterpret_cast<unsigned*>(a + 160) = 0;
        } else if (mode == 2) {
            *reinterpret_cast<float*>(a + 156) = 1.0f;
        }
        unsigned char* position = a + 20;
        if (*reinterpret_cast<int*>(sprite + 52) != 5) {
            const Vec3* loc = GetActorLocation__C7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(self + 24));
            reinterpret_cast<float*>(position)[0] = loc->x;
            reinterpret_cast<float*>(position)[1] = loc->y;
            reinterpret_cast<float*>(position)[2] = loc->z;
            zeroVec(a + 92);
        } else {
            unsigned b = *reinterpret_cast<unsigned*>(record + 12) & 0xff;
            if (b == 0 || b == 1) {
                Vec3 p = nodePosition(self, *reinterpret_cast<int*>(self + 24), static_cast<int>(b), m0);
                storeVec(position, p);
                zeroVec(a + 92);
            } else if (b == 2) {
                Vec3 first = nodePosition(self, *reinterpret_cast<int*>(self + 24), 0, m0);
                w1 = first;
                Vec3 second = nodePosition(self, *reinterpret_cast<int*>(self + 24), 1, m0);
                w2 = second;
                Vec3 mid = w1;
                mid.x = mid.x + w2.x;
                mid.y = mid.y + w2.y;
                mid.z = mid.z + w2.z;
                mid.x = mid.x * 0.5f;
                mid.y = mid.y * 0.5f;
                mid.z = mid.z * 0.5f;
                storeVec(position, mid);
                zeroVec(a + 92);
            } else if (b == 3) {
                Vec3 first = nodePosition(self, *reinterpret_cast<int*>(self + 24), 0, m0);
                w1 = first;
                Vec3 second = nodePosition(self, *reinterpret_cast<int*>(self + 24), 1, m0);
                w2 = second;
                Vec3 mid = w1;
                mid.x = mid.x + w2.x;
                mid.y = mid.y + w2.y;
                mid.z = mid.z + w2.z;
                mid.x = mid.x * 0.5f;
                mid.y = mid.y * 0.5f;
                mid.z = mid.z * 0.5f;
                storeVec(position, mid);
                GetActorTransform__7VISceneiR10VIMatrix44(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(self + 24), m0);
                Transpose__10VIMatrix44RC10VIMatrix44(m1, m0);
                float t0 = -((m1[3] * m1[0] + m1[7] * m1[4]) + m1[11] * m1[8]);
                float t1 = -((m1[3] * m1[1] + m1[7] * m1[5]) + m1[11] * m1[9]);
                float t2 = -((m1[3] * m1[2] + m1[7] * m1[6]) + m1[11] * m1[10]);
                m1[7] = 0.0f;
                m1[3] = 0.0f;
                m1[11] = 0.0f;
                m1[12] = t0;
                m1[13] = t1;
                m1[14] = t2;
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, &w1, reinterpret_cast<Vec3*>(a + 44));
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, &w2, reinterpret_cast<Vec3*>(a + 56));
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, reinterpret_cast<Vec3*>(position), &vv);
                float* p0 = reinterpret_cast<float*>(a + 44);
                float* p1 = reinterpret_cast<float*>(a + 56);
                p0[0] = p0[0] - vv.x;
                p0[1] = p0[1] - vv.y;
                p0[2] = p0[2] - vv.z;
                p1[0] = p1[0] - vv.x;
                p1[1] = p1[1] - vv.y;
                p1[2] = p1[2] - vv.z;
                if (((*reinterpret_cast<int*>(record + 12) >> 12) & 1) != 0) {
                    unsigned char* sp3 = GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(self + 24));
                    if (sp3 == 0) {
                        _$_10VIMatrix44(m1, 2);
                        _$_10VIMatrix44(m0, 2);
                        return -1;
                    }
                    helperActor(self, sp3, reinterpret_cast<int*>(a + 144));
                }
                if (*reinterpret_cast<int*>(record + 8) >= 0) {
                    makeEmitter(self, record, a, 124);
                }
                reinterpret_cast<float*>(a + 92)[0] = p0[0];
                reinterpret_cast<float*>(a + 92)[1] = p0[1];
                reinterpret_cast<float*>(a + 92)[2] = p0[2];
                reinterpret_cast<float*>(a + 104)[0] = p1[0];
                reinterpret_cast<float*>(a + 104)[1] = p1[1];
                reinterpret_cast<float*>(a + 104)[2] = p1[2];
            }
        }
        reinterpret_cast<float*>(a + 32)[0] = reinterpret_cast<float*>(position)[0];
        *reinterpret_cast<unsigned char**>(d + 36) = a;
        reinterpret_cast<float*>(a + 32)[1] = reinterpret_cast<float*>(position)[1];
        reinterpret_cast<float*>(a + 32)[2] = reinterpret_cast<float*>(position)[2];
    }
    _$_10VIMatrix44(m1, 2);
    _$_10VIMatrix44(m0, 2);
    return 0;
}
}
