// Observed: ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera at 0x0106ace0 is 1572 bytes.
// Arguments: atmosphere A (a0), scene (a1), camera C (a2). Words are 32-bit; F[o] a float at byte
// offset o. Fields used: A: F[76] a storm intensity, the int at +4460 (live count), the word at +0
// (the particle system), ints +4480 and +4484 (two particle definition ids), the list head words at
// +4448 (active list) and +4464 (free list), both ending in a node whose first word is zero. C: the
// position floats at +28..+36. Node (the 1568+ byte items on those lists): floats +16..+24 start
// position, +28..+36 target position, +40 progress, +44 speed, int +48 state flag, an embedded
// VIParticleEmitter at +64 and another at +1056.
//   1. If the live count is below (int)(F[76] * 32.0f) one node is spawned: a point P = (x, 5000.0f,
//      z) with x = ((float)r1 - (float)r2) * 2^-31 * 50.0f and z likewise from two further rand()
//      calls. The values are converted to double with fptodp; when |x| < 10.0 and |z| < 10.0 (double
//      compares with dpcmp, negation with dpsub from zero) both are pushed away by 30.0f in their own
//      direction (x + 30.0f when x >= 0.0f, else x - 30.0f, z likewise). P becomes P + C.position
//      (x, y, z added separately); the ray runs from P to (P.x, P.y - 10000.0f, P.z).
//   2. The ray objects are a VICollRay, a VISceneFilter with the vtable word _vt$13WeatherFilter at
//      filter+16 and a VIEnvRay (constructed with __9VICollRay, __13VISceneFilter and __8VIEnvRay,
//      no destructor runs). VICollRay Init (P, end), VIEnvRay Init (P, end, 15). If
//      Collide__7VISceneR9VICollRayR13VISceneFilter(scene, ray, filter) is zero the spawn ends.
//      The hit point H comes from GetHitPoint__9VICollRay; when Collide__7VISceneR8VIEnvRay(scene,
//      envRay) is non-zero its hit point H2 (GetHitPoint__C8VIEnvRay) replaces H if H.y < H2.y.
//   3. node = RemHead__7VILList(A + 4464); null ends the spawn. The node is appended to the list at
//      A + 4448 with AddTail__7VILListP7VILNode and the live count is incremented. node+28 and node+16
//      get H, node+20 additionally gets + 40.0f, node+40 = 0 (int), node+48 = 0 (int), node+44 =
//      (((131 * rand()) & 0xff) * 0x3b808080 * 0x3f199999 + 0x3f333333) * 2.5f. Two emitters are
//      created: Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(node+64, system,
//      ParticleDefinition__16VIParticleSystemi(system, A[4480])), then SetLocation__17VIParticleEmitterRC7VIVect3(
//      node+64, node+16) twice; the same with node+1056, the definition A[4484] and node+28.
//   4. Every node of the active list whose next word is non-zero is updated: ParticleCount__C17VIParticleEmitter(node+64)
//      is called (the result is unused); progress = node[40] + DeltaTime__C16VIParticleSystem(system) *
//      0x3e199999 * node[44] is stored at node+40. Progress >= 1.0f: if node[48] is zero it calls
//      Update__17VIParticleEmitter(node+1056) and sets node[48] = 1; else, when ParticleCount(node+1056)
//      is zero, it calls Destroy__17VIParticleEmitter on node+64 and on node+1056, Remove__7VILNode(node),
//      AddTail__7VILListP7VILNode(A + 4464, node) and decrements the live count. Progress < 1.0f:
//      the position node[16..] + (node[28..] - node[16..]) * progress (componentwise, differences first)
//      goes to SetLocation__17VIParticleEmitterRC7VIVect3(node+64, ...) followed by
//      Update__17VIParticleEmitter(node+64).
// Inferred: lava rain falling from above the camera: one falling blob per call is placed on the
// ground found by the ray (excluding a 10-unit square around the camera), animated from 40 units up to
// the ground with two particle emitters, then recycled.
// Unknown: field names, the true size of the nodes, the meaning of node+48, and what the soft-float
// helpers return beyond an ordering (dpcmp) and a double (fptodp, dpsub); doubles are modelled as 64-bit
// integers. The sizes of the three stack objects (96, 32 and 64 bytes) are upper bounds taken from
// the original frame layout.
extern "C" {
unsigned long long fptodp(float value);
int dpcmp(unsigned long long a, unsigned long long b);
unsigned long long dpsub(unsigned long long a, unsigned long long b);
int rand(void);
void __9VICollRay(unsigned char* ray);
void __13VISceneFilter(unsigned char* filter);
void __8VIEnvRay(unsigned char* ray);
void Init__9VICollRayRC7VIVect3T1(unsigned char* ray, const float* from, const float* to);
void Init__8VIEnvRayRC7VIVect3T1Ui(unsigned char* ray, const float* from, const float* to, unsigned flags);
int Collide__7VISceneR9VICollRayR13VISceneFilter(unsigned char* scene, unsigned char* ray, unsigned char* filter);
int Collide__7VISceneR8VIEnvRay(unsigned char* scene, unsigned char* ray);
void GetHitPoint__9VICollRay(float* out, unsigned char* ray);
void GetHitPoint__C8VIEnvRay(float* out, unsigned char* ray);
void* RemHead__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);
void* ParticleDefinition__16VIParticleSystemi(void* system, int id);
void Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(void* emitter, void* system, void* definition);
void SetLocation__17VIParticleEmitterRC7VIVect3(void* emitter, const float* location);
int ParticleCount__C17VIParticleEmitter(const void* emitter);
float DeltaTime__C16VIParticleSystem(const void* system);
void Update__17VIParticleEmitter(void* emitter);
void Destroy__17VIParticleEmitter(void* emitter);
void Remove__7VILNode(void* node);
extern unsigned char _vt$13WeatherFilter[];
}

// Words that hold 32-bit target addresses: a host check defines NORRATH_HOST_ARENA, so they hold
// arena offsets and the helpers below are provided by the check.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_word(const void* pointer);
#define WORD_POINTER(word) norrath_host_pointer(word)
#define POINTER_WORD(pointer) norrath_host_word(pointer)
#else
#define WORD_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define POINTER_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

static inline float K(unsigned bits) { union { unsigned u; float f; } v; v.u = bits; return v.f; }
static inline float& F(unsigned char* base, unsigned offset) { return *reinterpret_cast<float*>(base + offset); }
static inline int& I(unsigned char* base, unsigned offset) { return *reinterpret_cast<int*>(base + offset); }
static inline unsigned char* pointerAt(unsigned char* base, unsigned offset) {
    return WORD_POINTER(*reinterpret_cast<unsigned*>(base + offset));
}

extern "C" {
void ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera(unsigned char* self, unsigned char* scene, unsigned char* camera) {
    const unsigned long long zero = 0;
    const unsigned long long ten = 0x4024000000000000ull;
    if (I(self, 4460) < static_cast<int>(F(self, 76) * K(0x42000000))) {
        float p[3] = {0.0f, 0.0f, 0.0f};
        float q[3];
        q[0] = 0.0f;
        q[1] = 0.0f;
        q[2] = 0.0f;
        int r1 = rand();
        int r2 = rand();
        p[1] = K(0x459c4000);
        p[0] = (static_cast<float>(r1) - static_cast<float>(r2)) * K(0x30000000) * K(0x42480000);
        int r3 = rand();
        int r4 = rand();
        float z = (static_cast<float>(r3) - static_cast<float>(r4)) * K(0x30000000) * K(0x42480000);
        p[2] = z;
        unsigned long long magnitude = fptodp(p[0]);
        if (dpcmp(magnitude, zero) < 0) {
            magnitude = dpsub(zero, magnitude);
        }
        if (dpcmp(magnitude, ten) < 0) {
            magnitude = fptodp(z);
            if (dpcmp(magnitude, zero) < 0) {
                magnitude = dpsub(zero, magnitude);
            }
            if (dpcmp(magnitude, ten) < 0) {
                float x = p[0];
                p[0] = x < 0.0f ? x + K(0xc1f00000) : x + K(0x41f00000);
                float zz = p[2];
                p[2] = zz < 0.0f ? zz + K(0xc1f00000) : zz + K(0x41f00000);
            }
        }
        const float* cameraPosition = reinterpret_cast<const float*>(camera + 28);
        float x = p[0] + cameraPosition[0];
        float y = p[1] + cameraPosition[1];
        float zc = p[2] + cameraPosition[2];
        p[0] = x;
        p[1] = y;
        p[2] = zc;
        q[0] = x;
        q[1] = p[1] - K(0x461c4000);
        q[2] = p[2];
        alignas(16) unsigned char ray[96];
        alignas(16) unsigned char filter[32];
        alignas(16) unsigned char envRay[64];
        float hit[3];
        float hit2[3];
        __9VICollRay(ray);
        __13VISceneFilter(filter);
        *reinterpret_cast<unsigned*>(filter + 16) = POINTER_WORD(_vt$13WeatherFilter);
        __8VIEnvRay(envRay);
        Init__9VICollRayRC7VIVect3T1(ray, p, q);
        Init__8VIEnvRayRC7VIVect3T1Ui(envRay, p, q, 15);
        if (Collide__7VISceneR9VICollRayR13VISceneFilter(scene, ray, filter) != 0) {
            GetHitPoint__9VICollRay(hit, ray);
            if (Collide__7VISceneR8VIEnvRay(scene, envRay) != 0) {
                GetHitPoint__C8VIEnvRay(hit2, envRay);
                if (hit[1] < hit2[1]) {
                    hit[0] = hit2[0];
                    hit[1] = hit2[1];
                    hit[2] = hit2[2];
                }
            }
            unsigned char* node = static_cast<unsigned char*>(RemHead__7VILList(self + 4464));
            if (node != 0) {
                AddTail__7VILListP7VILNode(self + 4448, node);
                float* target = reinterpret_cast<float*>(node + 28);
                float* start = reinterpret_cast<float*>(node + 16);
                I(self, 4460) = I(self, 4460) + 1;
                target[0] = hit[0];
                target[1] = hit[1];
                target[2] = hit[2];
                start[0] = hit[0];
                start[1] = hit[1];
                start[2] = hit[2];
                I(node, 40) = 0;
                F(node, 20) = F(node, 20) + K(0x42200000);
                int roll = rand();
                I(node, 48) = 0;
                void* system = pointerAt(self, 0);
                F(node, 44) = (static_cast<float>((131 * roll) & 0xff) * K(0x3b808080) * K(0x3f199999) + K(0x3f333333)) * K(0x40200000);
                void* definition = ParticleDefinition__16VIParticleSystemi(system, I(self, 4480));
                Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(node + 64, system, definition);
                SetLocation__17VIParticleEmitterRC7VIVect3(node + 64, start);
                SetLocation__17VIParticleEmitterRC7VIVect3(node + 64, start);
                definition = ParticleDefinition__16VIParticleSystemi(pointerAt(self, 0), I(self, 4484));
                Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(node + 1056, pointerAt(self, 0), definition);
                SetLocation__17VIParticleEmitterRC7VIVect3(node + 1056, target);
                SetLocation__17VIParticleEmitterRC7VIVect3(node + 1056, target);
            }
        }
    }

    unsigned char* node = pointerAt(self, 4448);
    unsigned char* next = pointerAt(node, 0);
    if (next != 0) {
        do {
            unsigned char* emitter = node + 64;
            ParticleCount__C17VIParticleEmitter(emitter);
            float delta = DeltaTime__C16VIParticleSystem(pointerAt(self, 0));
            const float* target = reinterpret_cast<const float*>(node + 28);
            const float* start = reinterpret_cast<const float*>(node + 16);
            float speed = F(node, 44);
            delta = delta * K(0x3e199999);
            delta = delta * speed;
            float progress = F(node, 40) + delta;
            F(node, 40) = progress;
            float location[3];
            if (K(0x3f800000) <= progress) {
                if (I(node, 48) == 0) {
                    I(node, 48) = 1;
                    Update__17VIParticleEmitter(node + 1056);
                } else if (ParticleCount__C17VIParticleEmitter(node + 1056) == 0) {
                    Destroy__17VIParticleEmitter(emitter);
                    Destroy__17VIParticleEmitter(node + 1056);
                    Remove__7VILNode(node);
                    AddTail__7VILListP7VILNode(self + 4464, node);
                    I(self, 4460) = I(self, 4460) - 1;
                }
            } else {
                float dx = target[0] - start[0];
                float dz = target[2] - start[2];
                float dy = target[1] - start[1];
                location[0] = dx;
                location[2] = dz;
                location[1] = dy;
                float t = F(node, 40);
                location[0] = location[0] * t;
                location[1] = location[1] * t;
                location[2] = location[2] * t;
                location[0] = location[0] + start[0];
                location[1] = location[1] + start[1];
                location[2] = location[2] + start[2];
                SetLocation__17VIParticleEmitterRC7VIVect3(emitter, location);
                Update__17VIParticleEmitter(emitter);
            }
            node = next;
            next = pointerAt(node, 0);
        } while (next != 0);
    }
}
}
