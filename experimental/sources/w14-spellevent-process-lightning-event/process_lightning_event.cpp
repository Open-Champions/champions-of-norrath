// Observed: ProcessLightningEvent__18VISpellEventDriver at 0x0117fca8 is 1992 bytes in the pinned
// SLUS-20565 image. Argument: this. Returns -1 in one case, 0 otherwise. On entry it builds on the
// stack: a zeroed 4x4 float matrix m0 (destructor _$_10VIMatrix44(m0, 2) on every path), zero float
// triples for two end points p[0], p[1], a mid point q, and a pair done[2] initialised from the two
// ints at 0x011de010 (1, 1 in the image). w = the word at this+48 (the active lightning event); ev =
// the word at w+116 (its script record); actor = the int at w+140; id = the int at this+24; scene = the
// word at this+20; raster = the word at scene+19356. T(n, a) below means GetNodeTransform__18VISpell
// EventDriveri15VICSpriteNodeIDR10VIMatrix44(this, a, n, &m0) followed by reading the position
// m0[12..14].
//   1. When id or actor is -1: done[0] = done[1] = 0 and the function continues at step 5.
//   2. sprite = GetActorSpritePtr__C7VIScenei(scene, id); null: it returns -1 (after the matrix
//      destructor) without step 5. By the unsigned byte at ev+8: 0: p[0] = T(0, actor); 1: p[0] =
//      T(1, actor); 2: p[0] = (T(0, actor) + T(1, actor)) * 0.5 (componentwise, the sum taken as
//      p[0] = first; p[0] += second; p[0] *= 0.5); 3: p[0] = T(0, actor) and p[1] = T(1, actor);
//      other values leave both at zero.
//   3. q = (T(0, id) + T(1, id)) * 0.5 with id the int at this+24 re-read.
//   4. For lightning 0 (the word at this+40, created first with __builtin_new(8832) and __11VILightning
//      when it is zero) and, only when the byte at ev+8 is 3, lightning 1 (the word at this+44, created
//      the same way when zero): let past = (the int at ev+12) < (low 32 bits of GetTime__7VIScene(scene)
//      minus the 64-bit value at w+176). For lightning 0, unless past, SetCharge__11VILightningf(l0, 1.0f)
//      and, if lightning 1 exists, SetCharge(l1, 1.0f). Then for each existing lightning n with end
//      point p[n]: SetColor__11VILightningRC10VIColor32F(ln, {ev[20], ev[24], ev[28], 1.0f}) (the floats
//      at ev+20, +24, +28); SetLocation__11VILightningRC7VIVect3T1(ln, p[n], q); SetMaxBranchCount__11
//      VILightningi(ln, the int at ev+16); Update__11VILightningi(ln, (int)(DeltaTime__C16VIParticle
//      System(scene + 1352) * 1000.0f)); Render__11VILightningR8VIRaster(ln, raster); and done[n] = 1
//      when Charge__C11VILightning(ln) is above 0.0f, else 0. (Lightning 1 uses a second DeltaTime call.)
//   5. When the int at ev+12 is below (low 32 bits of GetTime(scene) minus the 64-bit value at w+176)
//      and (done[0] is 0 or done[1] is 0): the event is torn down: -1 is stored at w+140; if w is non-zero
//      ReleaseActor__7VIScenei(the word at w+16, the int at w+144) when that int is not -1,
//      _$_17VIParticleEmitter(it, 3) for the non-zero words at w+120 and w+124, _$_14VIMotifBlender(it,
//      3) for the word at w+128, __builtin_delete(w); then zero is stored at this+48; and each existing
//      lightning (this+40, this+44) is destroyed with _$_11VILightning(it, 3) and its word cleared.
//      Returns 0.
// Inferred: record type 13 (target lightning) step: the end points are the target node positions
// (kind 0 and 1 one of the sprite's two nodes, kind 2 the middle of both, kind 3 both ends for two
// bolts), the origin is the middle of this driver's actor's two nodes; the bolt is drawn each frame and
// removed once its time is up and it has discharged; done[n] is 1 while lightning n still has charge.
// Unknown: node numbers 0 and 1 (probably hand and hand/target slots), the charge scale, why SetCharge
// is called only before the duration has passed, the 1000.0f factor on DeltaTime, and whether the
// returned -1 is read. Nothing here ran on the EE.
struct Vec3 {
    float x, y, z;
};
struct Color4 {
    float a, b, c, d;
};
extern "C" {
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
void GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44(void* driver, int actor, int node, void* matrix);
void* __builtin_new(unsigned size);
void __11VILightning(void* lightning);
void _$_11VILightning(void* lightning, int flags);
void SetCharge__11VILightningf(void* lightning, float charge);
void SetColor__11VILightningRC10VIColor32F(void* lightning, const Color4* color);
void SetLocation__11VILightningRC7VIVect3T1(void* lightning, const Vec3* from, const Vec3* to);
void SetMaxBranchCount__11VILightningi(void* lightning, int count);
float DeltaTime__C16VIParticleSystem(void* system);
void Update__11VILightningi(void* lightning, int ticks);
void Render__11VILightningR8VIRaster(void* lightning, void* raster);
float Charge__C11VILightning(const void* lightning);
long long GetTime__7VIScene(void* scene);
void ReleaseActor__7VIScenei(void* scene, int actor);
void _$_17VIParticleEmitter(void* emitter, int flags);
void _$_14VIMotifBlender(void* blender, int flags);
void __builtin_delete(void* pointer);
void _$_10VIMatrix44(void* matrix, int flags);

static inline Vec3 nodePosition(unsigned char* self, int actor, int node, float* m) {
    GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44(self, actor, node, m);
    Vec3 v;
    v.x = m[12];
    v.y = m[13];
    v.z = m[14];
    return v;
}

static inline void driveLightning(unsigned char* self, unsigned char* ev, void* lightning, const Vec3* from, const Vec3* to, void* raster) {
    Color4 color;
    color.a = *reinterpret_cast<float*>(ev + 20);
    color.b = *reinterpret_cast<float*>(ev + 24);
    color.c = *reinterpret_cast<float*>(ev + 28);
    color.d = 1.0f;
    SetColor__11VILightningRC10VIColor32F(lightning, &color);
    SetLocation__11VILightningRC7VIVect3T1(lightning, from, to);
    SetMaxBranchCount__11VILightningi(lightning, *reinterpret_cast<int*>(ev + 16));
    Update__11VILightningi(lightning, static_cast<int>(DeltaTime__C16VIParticleSystem(*reinterpret_cast<unsigned char**>(self + 20) + 1352) * 1000.0f));
    Render__11VILightningR8VIRaster(lightning, raster);
}

int ProcessLightningEvent__18VISpellEventDriver(unsigned char* self) {
    alignas(16) float m0[16];
    for (int i = 0; i < 16; ++i) {
        m0[i] = 0.0f;
    }
    Vec3 p[2] = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    Vec3 q = {0.0f, 0.0f, 0.0f};
    int done[2] = {1, 1};
    unsigned char* w = *reinterpret_cast<unsigned char**>(self + 48);
    unsigned char* scene = *reinterpret_cast<unsigned char**>(self + 20);
    int id = *reinterpret_cast<int*>(self + 24);
    int actor = *reinterpret_cast<int*>(w + 140);
    void* raster = *reinterpret_cast<void**>(scene + 19356);
    unsigned char* ev = *reinterpret_cast<unsigned char**>(w + 116);
    if (id == -1 || actor == -1) {
        done[0] = 0;
        done[1] = 0;
    } else {
        void* sprite = GetActorSpritePtr__C7VIScenei(scene, id);
        if (sprite == 0) {
            _$_10VIMatrix44(m0, 2);
            return -1;
        }
        unsigned kind = ev[8];
        if (kind == 0) {
            p[0] = nodePosition(self, actor, 0, m0);
        } else if (kind == 1) {
            p[0] = nodePosition(self, actor, 1, m0);
        } else if (kind == 2) {
            Vec3 first = nodePosition(self, actor, 0, m0);
            Vec3 second = nodePosition(self, actor, 1, m0);
            p[0] = first;
            p[0].x = p[0].x + second.x;
            p[0].y = p[0].y + second.y;
            p[0].z = p[0].z + second.z;
            p[0].x = p[0].x * 0.5f;
            p[0].y = p[0].y * 0.5f;
            p[0].z = p[0].z * 0.5f;
        } else if (kind == 3) {
            p[0] = nodePosition(self, actor, 0, m0);
            p[1] = nodePosition(self, actor, 1, m0);
        }
        Vec3 c = nodePosition(self, *reinterpret_cast<int*>(self + 24), 0, m0);
        Vec3 d = nodePosition(self, *reinterpret_cast<int*>(self + 24), 1, m0);
        q = c;
        q.x = q.x + d.x;
        q.y = q.y + d.y;
        q.z = q.z + d.z;
        q.x = q.x * 0.5f;
        q.y = q.y * 0.5f;
        q.z = q.z * 0.5f;
        if (*reinterpret_cast<void**>(self + 40) == 0) {
            void* created = __builtin_new(8832);
            if (created != 0) {
                __11VILightning(created);
            }
            *reinterpret_cast<void**>(self + 40) = created;
        }
        if (ev[8] == 3 && *reinterpret_cast<void**>(self + 44) == 0) {
            void* created = __builtin_new(8832);
            if (created != 0) {
                __11VILightning(created);
            }
            *reinterpret_cast<void**>(self + 44) = created;
        }
        long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
        if (!(*reinterpret_cast<int*>(ev + 12) < static_cast<int>(now - *reinterpret_cast<long long*>(*reinterpret_cast<unsigned char**>(self + 48) + 176)))) {
            SetCharge__11VILightningf(*reinterpret_cast<void**>(self + 40), 1.0f);
            if (*reinterpret_cast<void**>(self + 44) != 0) {
                SetCharge__11VILightningf(*reinterpret_cast<void**>(self + 44), 1.0f);
            }
        }
        driveLightning(self, ev, *reinterpret_cast<void**>(self + 40), &p[0], &q, raster);
        done[0] = Charge__C11VILightning(*reinterpret_cast<void**>(self + 40)) > 0.0f ? 1 : 0;
        if (*reinterpret_cast<void**>(self + 44) != 0) {
            driveLightning(self, ev, *reinterpret_cast<void**>(self + 44), &p[1], &q, raster);
            done[1] = Charge__C11VILightning(*reinterpret_cast<void**>(self + 44)) > 0.0f ? 1 : 0;
        }
    }
    long long end = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    unsigned char* active = *reinterpret_cast<unsigned char**>(self + 48);
    if (*reinterpret_cast<int*>(ev + 12) < static_cast<int>(end - *reinterpret_cast<long long*>(active + 176))) {
        if (done[0] == 0 || done[1] == 0) {
            *reinterpret_cast<int*>(active + 140) = -1;
            if (active != 0) {
                if (*reinterpret_cast<int*>(active + 144) != -1) {
                    ReleaseActor__7VIScenei(*reinterpret_cast<void**>(active + 16), *reinterpret_cast<int*>(active + 144));
                }
                if (*reinterpret_cast<void**>(active + 120) != 0) {
                    _$_17VIParticleEmitter(*reinterpret_cast<void**>(active + 120), 3);
                }
                if (*reinterpret_cast<void**>(active + 124) != 0) {
                    _$_17VIParticleEmitter(*reinterpret_cast<void**>(active + 124), 3);
                }
                if (*reinterpret_cast<void**>(active + 128) != 0) {
                    _$_14VIMotifBlender(*reinterpret_cast<void**>(active + 128), 3);
                }
                __builtin_delete(active);
            }
            *reinterpret_cast<unsigned*>(self + 48) = 0;
            if (*reinterpret_cast<void**>(self + 40) != 0) {
                _$_11VILightning(*reinterpret_cast<void**>(self + 40), 3);
                *reinterpret_cast<unsigned*>(self + 40) = 0;
            }
            if (*reinterpret_cast<void**>(self + 44) != 0) {
                _$_11VILightning(*reinterpret_cast<void**>(self + 44), 3);
                *reinterpret_cast<unsigned*>(self + 44) = 0;
            }
        }
    }
    _$_10VIMatrix44(m0, 2);
    return 0;
}
}
