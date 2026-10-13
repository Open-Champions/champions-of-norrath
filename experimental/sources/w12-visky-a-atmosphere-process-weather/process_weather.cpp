// Observed: ProcessWeather__12VIAtmosphereR7VISceneR8VICamera at 0x01068780 is 2632 bytes. The
// atmosphere (this, A), the scene and the camera (C) are the arguments. Words are 32-bit; F[o] is a float
// at byte offset o and W[o] a word. Fields used, with the roles inferred only from how they are used:
//  - A: F[4] the cell size R, W[8] a signed drop count N, W[12] a pointer to N 36-byte drop records,
//    F[60..68] the last camera-relative origin O, F[72] the weather intensity, F[76] the lava storm
//    intensity, F[80..104) K1 (two 3-float rows), F[104..128) K2 (two 3-float rows), F[128] a rate, F[132]
//    a phase, 64 wind rows of three floats at +168, 64 floats at +936, and the frame time dt at F[1192].
//  - C: floats at +8, +12, +16 (a direction D) and +28, +32, +36 (a position P).
//  - drop record: float x at +0, y at +4, z at +8 (relative to O), the ground height at +28 and a word at +32.
// Steps:
//  1. If F[76] > 0, ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera(A, scene, C) runs. If F[72] is 0
//     the routine ends here.
//  2. Locals: a VICollRay, a VISceneFilter-derived filter whose vtable word at filter+16 is
//     _vt$13WeatherFilter, and a VIEnvRay, each built by its constructor (__9VICollRay, __13VISceneFilter,
//     __8VIEnvRay); no destructor runs. N' = P + 5 * D (componentwise: D.x * 5.0 first, then + P.x). The delta
//     V = N' - O is formed per component and O is replaced by N'. count1 = (int)(F[72] * (float)N) and
//     count2 = (int)((float)count1 * F[128]).
//  3. If F[4] < V.x, F[4] < V.y or F[4] < V.z (not absolute values), V becomes (0, 0, 0) and every drop is
//     reseeded: A0 = drop + (0, 5000.0, 0) + O and B0 = drop + (0, -5000.0, 0) + O (each of x, y, z added
//     separately), VICollRay Init (A0, B0), VIEnvRay Init (A0, B0, 15), and when
//     Collide__7VISceneR9VICollRayR13VISceneFilter(scene, ray, filter) is nonzero: the hit point H1 comes
//     from GetHitPoint__9VICollRay, the drop's word at +32 is set to 0, and when Collide__7VISceneR8VIEnvRay
//     is nonzero its hit point H2 replaces H1 if H1.y < H2.y and the word at +32 becomes 1. Then drop+28 =
//     H1.y, drop+4 = H1.y - F[64], and a random byte b = (131 * rand()) & 255 mixes y: h = R * 0.5,
//     y = h + (y - h) * ((float)b * 0x3b808080).
//  4. The phase F[132] advances by dt * 10 and loses 64 when above 64. W0 = (int)(old phase).
//  5. For each drop i (N re-read each pass) with q = (i < count2) and w = (W0 + 29 * i + i) & 63:
//     x += ((wind[w].x * K1[q].x) + K2[q].x) * (dt * 60);
//     y += ((wind[w].y * K1[q].y) + (K2[q].y * T[i & 63])) * (dt * 60); z likewise to x with the z columns.
//     Then V is subtracted from the drop (y first, then x and z). Each axis wraps by +R when below -R/2
//     and -R when at or above R/2; a wrap of x or z (not of y) redoes the ray test as in step 3.
//     When the drop's y + F[64] is below its ground height: if q is 0 and i < count1 a splash is spawned at
//     (x + O.x, ground + 0.08, z + O.z) with AddSplash__12VIAtmosphereR7VIVect3; if the drop's word at +32
//     is 1, x and z are re-randomized as (b/255) * R - R/2 from two rand() calls; y becomes
//     max(R/2, ground - F[64]).
// Inferred: rain or snow particles tracked around the camera. Unknown: member names, the sizes of the
// three stack objects (96, 32 and 64 bytes are assumed from the original frame layout) and whether the
// word at +32 means "hit water".
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera(unsigned char* self, unsigned char* scene, unsigned char* camera);
void AddSplash__12VIAtmosphereR7VIVect3(unsigned char* self, unsigned char* position);
void __9VICollRay(unsigned char* ray);
void __13VISceneFilter(unsigned char* filter);
void __8VIEnvRay(unsigned char* ray);
void Init__9VICollRayRC7VIVect3T1(unsigned char* ray, unsigned char* from, unsigned char* to);
void Init__8VIEnvRayRC7VIVect3T1Ui(unsigned char* ray, unsigned char* from, unsigned char* to, unsigned flags);
int Collide__7VISceneR9VICollRayR13VISceneFilter(unsigned char* scene, unsigned char* ray, unsigned char* filter);
int Collide__7VISceneR8VIEnvRay(unsigned char* scene, unsigned char* ray);
void GetHitPoint__9VICollRay(unsigned char* out, unsigned char* ray);
void GetHitPoint__C8VIEnvRay(unsigned char* out, unsigned char* ray);
int rand(void);
extern unsigned char _vt$13WeatherFilter[];

static inline float K(unsigned bits) { union { unsigned u; float f; } v; v.u = bits; return v.f; }
static inline float& F(unsigned char* base, unsigned offset) { return *reinterpret_cast<float*>(base + offset); }
static inline unsigned& W(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }
static inline unsigned char* drops(unsigned char* self) { return NORRATH_WORD_TO_POINTER(W(self, 12)); }

// Casts a vertical segment through a drop (relative to the origin O) and, on a hit, sets the drop's
// ground height, relative height and flag word, then mixes its height with a random fraction.
static void cast_drop(unsigned char* self, unsigned char* scene, unsigned drop_offset, unsigned char* ray, unsigned char* env_ray,
                      unsigned char* filter, unsigned char* from, unsigned char* to, unsigned char* hit1, unsigned char* hit2) {
    unsigned char* origin = self + 60;
    const float ox = F(origin, 0), oy = F(origin, 4), oz = F(origin, 8);
    unsigned char* drop = drops(self) + drop_offset;
    F(from, 0) = F(drop, 0);
    F(from, 4) = F(drop, 4);
    F(from, 4) = F(from, 4) + K(0x459c4000);
    F(from, 8) = F(drop, 8);
    F(to, 0) = F(drop, 0);
    F(to, 4) = F(drop, 4);
    F(to, 4) = F(to, 4) - K(0x459c4000);
    F(to, 8) = F(drop, 8);
    F(from, 0) = F(from, 0) + ox;
    F(from, 4) = F(from, 4) + oy;
    F(from, 8) = F(from, 8) + oz;
    F(to, 0) = F(to, 0) + ox;
    F(to, 4) = F(to, 4) + oy;
    F(to, 8) = F(to, 8) + oz;
    Init__9VICollRayRC7VIVect3T1(ray, from, to);
    Init__8VIEnvRayRC7VIVect3T1Ui(env_ray, from, to, 15);
    if (Collide__7VISceneR9VICollRayR13VISceneFilter(scene, ray, filter) == 0) return;
    GetHitPoint__9VICollRay(hit1, ray);
    W(drops(self) + drop_offset, 32) = 0;
    float ground_y;
    unsigned char* d;
    if (Collide__7VISceneR8VIEnvRay(scene, env_ray) != 0) {
        GetHitPoint__C8VIEnvRay(hit2, env_ray);
        if (F(hit1, 4) < F(hit2, 4)) {
            F(hit1, 0) = F(hit2, 0);
            d = drops(self) + drop_offset;
            ground_y = F(hit2, 4);
            W(d, 32) = 1;
            F(hit1, 8) = F(hit2, 8);
            F(hit1, 4) = F(hit2, 4);
        } else {
            d = drops(self) + drop_offset;
            ground_y = F(hit1, 4);
        }
    } else {
        d = drops(self) + drop_offset;
        ground_y = F(hit1, 4);
    }
    F(d, 28) = ground_y;
    F(d, 4) = F(hit1, 4) - F(self, 64);
    const int r = rand();
    d = drops(self) + drop_offset;
    const float half = F(self, 4) * K(0x3f000000);
    const float t = F(d, 4) - half;
    const float u = static_cast<float>((r * 131) & 0xff) * K(0x3b808080);
    F(d, 4) = half + t * u;
}

void ProcessWeather__12VIAtmosphereR7VISceneR8VICamera(unsigned char* self, unsigned char* scene, unsigned char* camera) {
    if (0.0f < F(self, 76)) ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera(self, scene, camera);
    if (F(self, 72) == 0.0f) return;

    float delta[3] = {0.0f, 0.0f, 0.0f};
    alignas(16) unsigned char ray[96];
    alignas(16) unsigned char filter[32];
    alignas(16) unsigned char env_ray[64];
    float from[3] = {0.0f, 0.0f, 0.0f};
    float to[3] = {0.0f, 0.0f, 0.0f};
    float look[3] = {0.0f, 0.0f, 0.0f};
    float hit1[3], hit2[3];
    __9VICollRay(ray);
    __13VISceneFilter(filter);
    W(filter, 16) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13WeatherFilter));
    __8VIEnvRay(env_ray);
    unsigned char* origin = self + 60;
    look[0] = F(camera, 8); look[1] = F(camera, 12); look[2] = F(camera, 16);
    const float count_f = static_cast<float>(static_cast<int>(W(self, 8)));
    look[0] = look[0] * K(0x40a00000);
    look[1] = look[1] * K(0x40a00000);
    look[2] = look[2] * K(0x40a00000);
    const float rate = F(self, 128);
    const int count1 = static_cast<int>(F(self, 72) * count_f);
    const float ny = look[1] + F(camera, 32);
    const float nz = look[2] + F(camera, 36);
    const float nx = look[0] + F(camera, 28);
    const int count2 = static_cast<int>(static_cast<float>(count1) * rate);
    delta[1] = ny - F(origin, 4);
    delta[2] = nz - F(origin, 8);
    delta[0] = nx - F(origin, 0);
    F(origin, 4) = ny;
    F(origin, 0) = nx;
    F(origin, 8) = nz;

    const float cell = F(self, 4);
    const int n0 = static_cast<int>(W(self, 8));
    if (cell < delta[0] || cell < delta[1] || cell < delta[2]) {
        delta[0] = 0.0f; delta[1] = 0.0f; delta[2] = 0.0f;
        for (int i = 0; i < n0; ++i) {
            cast_drop(self, scene, 36 * i, ray, env_ray, filter, reinterpret_cast<unsigned char*>(from), reinterpret_cast<unsigned char*>(to),
                      reinterpret_cast<unsigned char*>(hit1), reinterpret_cast<unsigned char*>(hit2));
            if (!(i + 1 < static_cast<int>(W(self, 8)))) break;
        }
    }

    const float dt = F(self, 1192);
    const float step10 = dt * K(0x41200000);
    const float phase = F(self, 132);
    const int wind0 = static_cast<int>(phase);
    const float step60 = dt * K(0x42700000);
    F(self, 132) = phase + step10;
    if (K(0x42800000) < phase + step10) F(self, 132) = (phase + step10) - K(0x42800000);
    if (static_cast<int>(W(self, 8)) <= 0) return;

    const float half = F(self, 4) * K(0x3f000000);
    const float neg_half = -half;
    int wind = wind0;
    for (int i = 0; i < static_cast<int>(W(self, 8)); ++i, wind += 29) {
        unsigned char* drop = drops(self) + 36 * i;
        const int q = i < count2 ? 1 : 0;
        const int w = (wind + i) & 0x3f;
        unsigned char* k1 = self + 80 + 12 * q;
        unsigned char* k2 = self + 104 + 12 * q;
        unsigned char* wv = self + 168 + 12 * w;
        const float y0 = F(drop, 4);
        float x = F(drop, 0);
        x = x + ((F(wv, 0) * F(k1, 0)) + F(k2, 0)) * step60;
        F(drop, 0) = x;
        F(drop, 4) = y0 + ((F(wv, 4) * F(k1, 4)) + (F(k2, 4) * F(self, 936 + 4 * (i & 0x3f)))) * step60;
        F(drops(self) + 36 * i, 8) = F(drops(self) + 36 * i, 8) + ((F(wv, 8) * F(k1, 8)) + F(k2, 8)) * step60;
        drop = drops(self) + 36 * i;
        bool wrapped = false;
        float y = F(drop, 4) - delta[1];
        F(drop, 4) = y;
        float z = F(drop, 8) - delta[2];
        x = F(drop, 0) - delta[0];
        F(drop, 0) = x;
        F(drop, 8) = z;
        if (y < neg_half) F(drop, 4) = y + F(self, 4);
        else if (half <= y) F(drop, 4) = y - F(self, 4);
        drop = drops(self) + 36 * i;
        x = F(drop, 0);
        if (x < neg_half) { wrapped = true; F(drop, 0) = x + F(self, 4); }
        else if (half <= x) { wrapped = true; F(drop, 0) = x - F(self, 4); }
        z = F(drop, 8);
        if (z < neg_half) { wrapped = true; F(drop, 8) = z + F(self, 4); }
        else if (half <= z) { wrapped = true; F(drop, 8) = z - F(self, 4); }
        if (wrapped) {
            cast_drop(self, scene, 36 * i, ray, env_ray, filter, reinterpret_cast<unsigned char*>(from), reinterpret_cast<unsigned char*>(to),
                      reinterpret_cast<unsigned char*>(hit1), reinterpret_cast<unsigned char*>(hit2));
        }
        drop = drops(self) + 36 * i;
        if (F(drop, 4) + F(self, 64) < F(drop, 28)) {
            if (q == 0 && i < count1) {
                float splash[3] = {0.0f, 0.0f, 0.0f};
                splash[0] = F(drop, 0) + F(self, 60);
                splash[1] = F(drop, 28) + K(0x3da3d70a);
                splash[2] = F(drop, 8) + F(self, 68);
                AddSplash__12VIAtmosphereR7VIVect3(self, reinterpret_cast<unsigned char*>(splash));
            }
            drop = drops(self) + 36 * i;
            if (W(drop, 32) == 1) {
                int r = rand();
                F(drops(self) + 36 * i, 0) = (static_cast<float>((r * 131) & 0xff) * K(0x3b808080)) * F(self, 4) - F(self, 4) * K(0x3f000000);
                r = rand();
                F(drops(self) + 36 * i, 8) = (static_cast<float>((r * 131) & 0xff) * K(0x3b808080)) * F(self, 4) - F(self, 4) * K(0x3f000000);
                drop = drops(self) + 36 * i;
            }
            float h = F(drop, 28) - F(self, 64);
            F(drop, 4) = h > half ? h : half;
        }
    }
}
}
