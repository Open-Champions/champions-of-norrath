// Observed: ProcessProjectileEvent__18VISpellEventDriver at 0x01180470 is 5376 bytes in the pinned
// SLUS-20565 image. Argument: this. Returns -1 when the sprite of the actor id is missing, 0 otherwise.
// w = the word at this+36 (the active projectile event), ev = the word at w+116 (its script record),
// id = the int at this+24, scene = the word at this+20. Locals: two zeroed 4x4 float matrices m0 and m1
// (destructors _$_10VIMatrix44(m1, 2) then (m0, 2)), float triples named by their stack offset in the
// image (v128, v144, v160, v176, v192, v208, v224, v240, v256 = (0, 1, 0), v272), and later a third
// matrix and further triples. pos = the floats at w+20, cur = the floats at w+32.
//   1. id == -1: skip to step 7 with "retire" set. Otherwise sprite = GetActorSpritePtr__C7VIScenei
//      (scene, id); null: the two matrix destructors and return -1.
//   2. GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44(this, id, 4, m0); P = the
//      position m0[12..14]; v176 = v144 = P; v128 = pos - P; mc = P - cur; dc = |mc| (Magnitude__C7VIVect3),
//      d0 = |v128|; v192 = P - pos (the aim vector), v272 = v192; X = cross(v256, v272), Y = cross(v272, X)
//      (v256 is overwritten with Y); Normalize__7VIVect3 on X (v240), Y (v256) and v272 in that order; m0
//      rows 0, 1 and 2 are X, Y, v272 with the fourth column of each row zero, m0[15] = 1.0f and
//      m0[12..14] = pos; s = min(d0 * 0.6f, 1.0f) (min.s), with 0.6f the image's value (bits 0x3f199999).
//   3. mode = (the int at ev+12 >> 8) & 15. Each mode builds v144 and v160, the world positions of the
//      two emitters (words w+120 and w+124), from the two offset triples at w+44 and w+56 (and the two
//      older/newer pairs at w+68/+92 and w+80/+104):
//      0: when w+124 is zero and the int at w+144 is negative, v144 = pos; otherwise v208 = triple at w+44
//         times s, v224 = triple at w+56 times s, v144 = Mul(m0, v208) and v160 = Mul(m0, v224)
//         (Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, in, out));
//      1: RotateHPR__10VIMatrix44RC7VIVect3(m1, (0, 0, w[156])); w[156] += ev[28] * 5.0f * DeltaTime__C16VI
//         ParticleSystem(scene + 1352); with an emitter 2 or a helper actor 2 the two triples at w+44 and
//         w+56 are scaled by s, rotated by m1 into v208 and v224 and carried into v144 and v160 by m0;
//         without them w[160] grows by 3.0f * DeltaTime, is capped by min.s against ev[24], and v144 =
//         Mul(m0, Mul(m1, (w[160] * s, 0, 0)));
//      2: with emitter 2 or helper actor 2 and w[156] >= 1.0f the triples w+92 and w+104 become the old ones
//         at w+68 and w+80 and are redrawn near w+44, w+48, w+52 and w+56, w+60, w+64 by (rand() - rand()) *
//         2^-31 times 0.2f (x, y of the first triple, all of the second except z) or 0.05f (z), then scaled
//         by ev[24], and w[156] = 0; then both pairs are interpolated by w[156], scaled by s, w[156] +=
//         15.0f * DeltaTime and v144, v160 = Mul(m0, ...); without them only one triple (w+92) is
//         redrawn from zero and v144 = Mul(m0, interpolation);
//      3: the offset triples times s through m0 (single emitter: v144 = pos), then an arc: with
//         m = |pos - cur|, v144.y and v160.y gain sinf(m / dc * 3.14159274f) * dc * (1/ev[20])^2 * dc;
//      other modes leave v144 = P and v160 = 0.
//   4. SetLocation__17VIParticleEmitterRC7VIVect3, SetDirection__17VIParticleEmitterRC7VIVect3(emitter,
//      v192) and Update__17VIParticleEmitter for each non-zero emitter word (w+120 with v144, w+124 with
//      v160).
//   5. When the ints at w+140 and w+144 are not both -1: build a look-at matrix m2 from the aim vector
//      (mode 3: v496 = normalised v192 * ev[20] * DeltaTime * 2 added to pos, minus cur, with an arc added
//      to y as in 3; other modes v496 = v192), Euler angles from it with ConvertToEulerZXY__C10VIMatrix44R7VIVect3,
//      and SetActorTransform__7VISceneiRC7VIVect3fT2(scene, actor, angles, 1.0f, v144 or v160) for each
//      non-negative actor (w+140 with v144, w+144 with v160); then the matrix destructor.
//   6. v192 = normalised v192 * ev[20] * DeltaTime; pos += v192; d1 = |pos - P|.
//   7. Retire when "retire" is set, or d0 <= d1 and d1 < 3.0f: AddTail__7VILListP7VILNode(this+88, w) and
//      the word at this+36 is cleared. Then the matrix destructors; return 0.
// Inferred: the per-frame flight of a projectile (record type 8): it steers toward the target node at P,
// places up to two emitters and two hidden helper actors on it, and ends once it is within 3 units and
// no longer approaching.
// Unknown: the mode meanings, the units of ev[20], ev[24], ev[28], the use of node 4, the arc formula's
// physical meaning, and what the helper actors do. Nothing here ran on the EE.
struct Vec3 {
    float x, y, z;
};
extern "C" {
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
void GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44(void* driver, int actor, int node, void* matrix);
float Magnitude__C7VIVect3(const Vec3* v);
void Normalize__7VIVect3(Vec3* v);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const Vec3* in, Vec3* out);
void RotateHPR__10VIMatrix44RC7VIVect3(void* matrix, const Vec3* angles);
float DeltaTime__C16VIParticleSystem(void* system);
int rand();
float sinf(float x);
void SetLocation__17VIParticleEmitterRC7VIVect3(void* emitter, const Vec3* position);
void SetDirection__17VIParticleEmitterRC7VIVect3(void* emitter, const Vec3* direction);
void Update__17VIParticleEmitter(void* emitter);
void Identity__10VIMatrix44(void* matrix);
void ConvertToEulerZXY__C10VIMatrix44R7VIVect3(const void* matrix, Vec3* angles);
void SetActorTransform__7VISceneiRC7VIVect3fT2(void* scene, int actor, const Vec3* angles, float scale, const Vec3* position);
void AddTail__7VILListP7VILNode(void* list, void* node);
void _$_10VIMatrix44(void* matrix, int flags);
}

// The image multiplies and adds with separate mul.s / add.s / sub.s; the barrier keeps each product rounded.
static inline float product(float a, float b) {
    float r = a * b;
    __asm__("" : "+f"(r));
    return r;
}
static inline float floatMin(float a, float b) {
    float r;
    __asm__("min.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}
static inline float* floatsAt(unsigned char* base, int offset) { return reinterpret_cast<float*>(base + offset); }

// cross(a, b) with the operand order of the image: (a.y b.z - a.z b.y, a.z b.x - a.x b.z, a.x b.y - a.y b.x)
static inline Vec3 cross(const Vec3& a, const Vec3& b) {
    Vec3 r;
    r.x = product(a.y, b.z) - product(a.z, b.y);
    r.y = product(-a.x, b.z) + product(a.z, b.x);
    r.z = product(a.x, b.y) - product(a.y, b.x);
    return r;
}

static inline Vec3 scaled(const Vec3& v, float s) {
    Vec3 r;
    r.x = product(v.x, s);
    r.y = product(v.y, s);
    r.z = product(v.z, s);
    return r;
}

// (rand() - rand()) * 2^-31 * k, the first call's value being the minuend
static inline float jitter(float k) {
    int a = rand();
    int b = rand();
    float d = static_cast<float>(a) - static_cast<float>(b);
    d = product(d, 0x1p-31f);
    return product(d, k);
}

static inline void interpolate(Vec3& out, const float* newer, const float* older, float t, float scale) {
    out.x = newer[0];
    out.y = newer[1];
    out.z = newer[2];
    out.x = out.x - older[0];
    out.y = out.y - older[1];
    out.z = out.z - older[2];
    out = scaled(out, t);
    out.x = out.x + older[0];
    out.y = out.y + older[1];
    out.z = out.z + older[2];
    out = scaled(out, scale);
}

extern "C" int ProcessProjectileEvent__18VISpellEventDriver(unsigned char* self) {
    alignas(16) float m0[16];
    alignas(16) float m1[16];
    for (int i = 0; i < 16; ++i) {
        m0[i] = 0.0f;
    }
    for (int i = 0; i < 16; ++i) {
        m1[i] = 0.0f;
    }
    Vec3 v128 = {0.0f, 0.0f, 0.0f};
    Vec3 v144 = {0.0f, 0.0f, 0.0f};
    Vec3 v160 = {0.0f, 0.0f, 0.0f};
    Vec3 v176 = {0.0f, 0.0f, 0.0f};
    Vec3 v192 = {0.0f, 0.0f, 0.0f};
    Vec3 v208 = {0.0f, 0.0f, 0.0f};
    Vec3 v224 = {0.0f, 0.0f, 0.0f};
    Vec3 v240 = {0.0f, 0.0f, 0.0f};
    Vec3 v256 = {0.0f, 1.0f, 0.0f};
    Vec3 v272 = {0.0f, 0.0f, 0.0f};
    bool retire = false;
    float d0 = 0.0f;
    float d1 = 0.0f;
    int id = *reinterpret_cast<int*>(self + 24);
    unsigned char* w = *reinterpret_cast<unsigned char**>(self + 36);
    unsigned char* scene = *reinterpret_cast<unsigned char**>(self + 20);
    if (id == -1) {
        retire = true;
    } else {
        if (GetActorSpritePtr__C7VIScenei(scene, id) == 0) {
            _$_10VIMatrix44(m1, 2);
            _$_10VIMatrix44(m0, 2);
            return -1;
        }
        GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44(self, id, 4, m0);
        float* pos = floatsAt(w, 20);
        float* cur = floatsAt(w, 32);
        unsigned char* ev = *reinterpret_cast<unsigned char**>(w + 116);
        Vec3 p = {m0[12], m0[13], m0[14]};
        v176 = p;
        v144 = p;
        v128.x = pos[0] - p.x;
        v128.y = pos[1] - p.y;
        v128.z = pos[2] - p.z;
        Vec3 toTarget;
        toTarget.x = p.x - cur[0];
        toTarget.y = p.y - cur[1];
        toTarget.z = p.z - cur[2];
        float dc = Magnitude__C7VIVect3(&toTarget);
        d0 = Magnitude__C7VIVect3(&v128);
        v192 = p;
        v192.x = v192.x - pos[0];
        v192.y = v192.y - pos[1];
        v192.z = v192.z - pos[2];
        v272 = v192;
        v240 = cross(v256, v272);
        v256 = cross(v272, v240);
        Normalize__7VIVect3(&v240);
        Normalize__7VIVect3(&v256);
        Normalize__7VIVect3(&v272);
        m0[11] = 0.0f;
        m0[3] = 0.0f;
        m0[7] = 0.0f;
        m0[0] = v240.x;
        m0[1] = v240.y;
        m0[2] = v240.z;
        m0[4] = v256.x;
        m0[5] = v256.y;
        m0[6] = v256.z;
        m0[8] = v272.x;
        m0[9] = v272.y;
        m0[10] = v272.z;
        const float scale = floatMin(product(d0, 0x1.333332p-1f), 1.0f);
        m0[15] = 1.0f;
        m0[12] = pos[0];
        m0[13] = pos[1];
        m0[14] = pos[2];
        int mode = (*reinterpret_cast<int*>(ev + 12) >> 8) & 15;
        unsigned char* emitter2 = *reinterpret_cast<unsigned char**>(w + 124);
        int helper2 = *reinterpret_cast<int*>(w + 144);
        float* offA = floatsAt(w, 44);
        float* offB = floatsAt(w, 56);
        float* old1 = floatsAt(w, 68);
        float* new1 = floatsAt(w, 92);
        float* old2 = floatsAt(w, 80);
        float* new2 = floatsAt(w, 104);
        float* phase = floatsAt(w, 156);
        if (mode == 0) {
            if (emitter2 == 0 && helper2 < 0) {
                v144.x = pos[0];
                v144.y = pos[1];
                v144.z = pos[2];
            } else {
                v208.x = offA[0];
                v208.y = offA[1];
                v208.z = offA[2];
                v224.x = offB[0];
                v224.y = offB[1];
                v224.z = offB[2];
                v208 = scaled(v208, scale);
                v224 = scaled(v224, scale);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v208, &v144);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v224, &v160);
            }
        } else if (mode == 1) {
            Vec3 angles = {0.0f, 0.0f, phase[0]};
            RotateHPR__10VIMatrix44RC7VIVect3(m1, &angles);
            float dt = DeltaTime__C16VIParticleSystem(scene + 1352);
            phase[0] = phase[0] + product(product(floatsAt(ev, 28)[0], 5.0f), dt);
            if (emitter2 != 0 || helper2 >= 0) {
                v144.x = offA[0];
                v144.y = offA[1];
                v144.z = offA[2];
                v160.x = offB[0];
                v160.y = offB[1];
                v160.z = offB[2];
                v144 = scaled(v144, scale);
                v160 = scaled(v160, scale);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, &v144, &v208);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, &v160, &v224);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v208, &v144);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v224, &v160);
            } else {
                float dt2 = DeltaTime__C16VIParticleSystem(scene + 1352);
                floatsAt(w, 160)[0] = floatsAt(w, 160)[0] + product(dt2, 3.0f);
                floatsAt(w, 160)[0] = floatMin(floatsAt(ev, 24)[0], floatsAt(w, 160)[0]);
                v144.x = 0.0f;
                v144.z = 0.0f;
                v144.y = 0.0f;
                v144.x = product(floatsAt(w, 160)[0], scale);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, &v144, &v160);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v160, &v144);
            }
        } else if (mode == 2) {
            if (emitter2 != 0 || helper2 >= 0) {
                if (1.0f <= phase[0]) {
                    old1[0] = new1[0];
                    old1[1] = new1[1];
                    old1[2] = new1[2];
                    old2[0] = new2[0];
                    old2[1] = new2[1];
                    old2[2] = new2[2];
                    new1[0] = offA[0] + jitter(0x1.999998p-3f);
                    new1[1] = offA[1] + jitter(0x1.999998p-3f);
                    new1[2] = offA[2] + jitter(0x1.999998p-5f);
                    new2[0] = offB[0] + jitter(0x1.999998p-3f);
                    new2[1] = offB[1] + jitter(0x1.999998p-3f);
                    new2[2] = offB[2] + jitter(0x1.999998p-5f);
                    float scaleA = floatsAt(ev, 24)[0];
                    new1[0] = product(new1[0], scaleA);
                    new1[1] = product(new1[1], scaleA);
                    new1[2] = product(new1[2], scaleA);
                    float scaleB = floatsAt(ev, 24)[0];
                    new2[0] = product(new2[0], scaleB);
                    new2[1] = product(new2[1], scaleB);
                    new2[2] = product(new2[2], scaleB);
                    phase[0] = 0.0f;
                }
                interpolate(v208, new1, old1, phase[0], scale);
                interpolate(v224, new2, old2, phase[0], scale);
                float dt = DeltaTime__C16VIParticleSystem(scene + 1352);
                phase[0] = phase[0] + product(dt, 15.0f);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v208, &v144);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v224, &v160);
            } else {
                if (1.0f <= phase[0]) {
                    old1[0] = new1[0];
                    old1[1] = new1[1];
                    old1[2] = new1[2];
                    new1[0] = jitter(0x1.999998p-3f);
                    new1[1] = jitter(0x1.999998p-3f);
                    new1[2] = jitter(0x1.999998p-5f);
                    float scaleA = floatsAt(ev, 24)[0];
                    new1[0] = product(new1[0], scaleA);
                    new1[1] = product(new1[1], scaleA);
                    new1[2] = product(new1[2], scaleA);
                    phase[0] = 0.0f;
                }
                interpolate(v160, new1, old1, phase[0], scale);
                float dt = DeltaTime__C16VIParticleSystem(scene + 1352);
                phase[0] = phase[0] + product(dt, 15.0f);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v160, &v144);
            }
        } else if (mode == 3) {
            if (emitter2 != 0 || helper2 >= 0) {
                v208.x = offA[0];
                v208.y = offA[1];
                v208.z = offA[2];
                v224.x = offB[0];
                v224.y = offB[1];
                v224.z = offB[2];
                v208 = scaled(v208, scale);
                v224 = scaled(v224, scale);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v208, &v144);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m0, &v224, &v160);
            } else {
                v144.x = pos[0];
                v144.y = pos[1];
                v144.z = pos[2];
            }
            Vec3 along;
            along.x = pos[0] - cur[0];
            along.y = pos[1] - cur[1];
            along.z = pos[2] - cur[2];
            float length = Magnitude__C7VIVect3(&along);
            float swing = sinf(product(length / dc, 0x1.921fb4p+1f));
            float inverse = 1.0f / floatsAt(ev, 20)[0];
            inverse = product(inverse, inverse);
            float arc = product(product(product(swing, dc), inverse), dc);
            v160.y = v160.y + arc;
            v144.y = v144.y + arc;
        }
        if (*reinterpret_cast<unsigned char**>(w + 120) != 0) {
            SetLocation__17VIParticleEmitterRC7VIVect3(*reinterpret_cast<void**>(w + 120), &v144);
            SetDirection__17VIParticleEmitterRC7VIVect3(*reinterpret_cast<void**>(w + 120), &v192);
            Update__17VIParticleEmitter(*reinterpret_cast<void**>(w + 120));
        }
        if (*reinterpret_cast<unsigned char**>(w + 124) != 0) {
            SetLocation__17VIParticleEmitterRC7VIVect3(*reinterpret_cast<void**>(w + 124), &v160);
            SetDirection__17VIParticleEmitterRC7VIVect3(*reinterpret_cast<void**>(w + 124), &v192);
            Update__17VIParticleEmitter(*reinterpret_cast<void**>(w + 124));
        }
        alignas(16) float m2[16];
        for (int i = 0; i < 16; ++i) {
            m2[i] = 0.0f;
        }
        Vec3 v448 = {0.0f, 0.0f, 0.0f};
        if (!(*reinterpret_cast<int*>(w + 140) == -1 && *reinterpret_cast<int*>(w + 144) == -1)) {
            Vec3 v464 = {0.0f, 0.0f, 0.0f};
            Vec3 v480 = {0.0f, 1.0f, 0.0f};
            Vec3 v496 = {0.0f, 0.0f, 0.0f};
            if (((*reinterpret_cast<int*>(ev + 12) >> 8) & 15) == 3) {
                v496 = v192;
                Normalize__7VIVect3(&v496);
                float speed = floatsAt(ev, 20)[0];
                v496.x = product(v496.x, speed);
                v496.y = product(v496.y, speed);
                v496.z = product(v496.z, speed);
                float dt = DeltaTime__C16VIParticleSystem(scene + 1352);
                v496 = scaled(v496, dt);
                v496.x = v496.x + v496.x;
                v496.y = v496.y + v496.y;
                v496.z = v496.z + v496.z;
                Vec3 target;
                target.x = pos[0] + v496.x;
                target.y = pos[1] + v496.y;
                target.z = pos[2] + v496.z;
                Vec3 ahead;
                ahead.x = target.x - cur[0];
                ahead.y = target.y - cur[1];
                ahead.z = target.z - cur[2];
                float length = Magnitude__C7VIVect3(&ahead);
                float swing = sinf(product(length / dc, 0x1.921fb4p+1f));
                float inverse = 1.0f / floatsAt(ev, 20)[0];
                inverse = product(inverse, inverse);
                float arc = product(product(product(swing, dc), inverse), dc);
                target.y = target.y + arc;
                v496.x = target.x - v144.x;
                v496.y = target.y - v144.y;
                v496.z = target.z - v144.z;
            } else {
                v496 = v192;
            }
            v464 = cross(v480, v496);
            v480 = cross(v496, v464);
            Normalize__7VIVect3(&v464);
            Normalize__7VIVect3(&v480);
            Normalize__7VIVect3(&v496);
            Identity__10VIMatrix44(m2);
            m2[0] = v464.x;
            m2[1] = v464.y;
            m2[2] = v464.z;
            m2[4] = v480.x;
            m2[5] = v480.y;
            m2[6] = v480.z;
            m2[8] = v496.x;
            m2[9] = v496.y;
            m2[10] = v496.z;
            ConvertToEulerZXY__C10VIMatrix44R7VIVect3(m2, &v448);
        }
        if (*reinterpret_cast<int*>(w + 140) >= 0) {
            SetActorTransform__7VISceneiRC7VIVect3fT2(scene, *reinterpret_cast<int*>(w + 140), &v448, 1.0f, &v144);
        }
        if (*reinterpret_cast<int*>(w + 144) >= 0) {
            SetActorTransform__7VISceneiRC7VIVect3fT2(scene, *reinterpret_cast<int*>(w + 144), &v448, 1.0f, &v160);
        }
        _$_10VIMatrix44(m2, 2);
        Normalize__7VIVect3(&v192);
        v192 = scaled(v192, floatsAt(ev, 20)[0]);
        v192 = scaled(v192, DeltaTime__C16VIParticleSystem(scene + 1352));
        pos[0] = pos[0] + v192.x;
        pos[1] = pos[1] + v192.y;
        pos[2] = pos[2] + v192.z;
        v128.x = pos[0];
        v128.y = pos[1];
        v128.z = pos[2];
        v128.x = v128.x - v176.x;
        v128.y = v128.y - v176.y;
        v128.z = v128.z - v176.z;
        d1 = Magnitude__C7VIVect3(&v128);
    }
    if (retire || (d0 <= d1 && d1 < 3.0f)) {
        AddTail__7VILListP7VILNode(self + 88, w);
        *reinterpret_cast<unsigned*>(self + 36) = 0;
    }
    _$_10VIMatrix44(m1, 2);
    _$_10VIMatrix44(m0, 2);
    return 0;
}
