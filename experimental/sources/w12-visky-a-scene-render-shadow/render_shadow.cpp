// Observed: RenderShadow__7VIScenei at 0x0113bed8 is 2300 bytes. Arguments: the scene (S) and an actor
// index. Words are 32-bit; F[o] is a float and W[o] a word at byte offset o of S. S holds a material word
// at +6272 (-1 for none), a VIQueryTriList at +6128 (its word at +0 is a signed count, its word at +8 a
// pointer to 40-byte triangle records) and the raster word at +19356. A triangle record has three
// 3-float vertices at +4, +16 and +28 (the leading word is not read).
//  1. Three 64-byte matrices M0, M1 and M2 are zero-filled, a 3-float location is zeroed, and
//     __13VIActorFilter runs on a filter object; six words {5, 6, 4, 7, 0, 1} (the 24 bytes at 0x011db500)
//     are then copied to filter+16. Whenever the routine returns, _$_10VIMatrix44(M2, 2), then M1, then M0
//     run, each with 2.
//  2. If W[6272] is -1: return -1. A six-vector corner array and a bounding box B (6 floats) are zeroed.
//     GetActorSpritePtr__C7VIScenei(S, actor) returning 0 also returns -1.
//  3. GetActorTransform__7VISceneiR10VIMatrix44(S, actor, M0); Invert__10VIMatrix44RC10VIMatrix44(M1, M0).
//     For i in 0..5: CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(sprite,
//     nodes[i], actor, S, M2); the translation floats at M2+48, +52, +56 become the location; then
//     Mul__C10VIMatrix44RC7VIVect3R7VIVect3(M1, location, corner[i]); for i >= 4 the corner is scaled by
//     0.5 (x, y, z). Init__6VIBBoxiP7VIVect3(B, 6, corners) builds the box, which is then grown by (0.2, 0, 0.2)
//     on the minimum and the maximum side (subtracting 0.2, 0.0, 0.2 and adding 0.2, 0.0, 0.2).
//  4. A second box O (6 floats, zero-filled) receives Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(B, M0, O).
//     With L = GetActorLocation__C7VIScenei(S, actor): O.min.y = L.y - 1.0, O.max.y = L.y + 0.5 (stored
//     after the Transform call), and Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f(S+6128, O, (0, 1, 0), 80.0)
//     then QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList(S, O, filter, S+6128).
//  5. SetMaterial__8VIRasteri(raster, W[6272]), SetZBias__8VIRasteri(raster, 15). For each triangle (the
//     count is re-read each pass): with minx = B[0], minz = B[8], inv_dx = 1 / (B[12] - B[0]), inv_dz = 1 /
//     (B[20] - B[8]) and each vertex lifted by 0.04 on y: u = ((((x * M1[0]) + (y' * M1[16])) + (z * M1[32])) +
//     M1[48] - minx) * inv_dx and v = ((((x * M1[8]) + (y' * M1[24])) + (z * M1[40])) + M1[56] - minz) *
//     inv_dz (floats at those byte offsets of M1); a = min(max(L.y - y', 0), 1). The fan is drawn with
//     BeginTriFan__8VIRaster, then for each vertex Color__8VIRasterRC10VIColor32F(c) with c = (1, 1, 1,
//     (1 - a) * 0.65), UV__8VIRasterRC7VIVect2 (u, v) and Vertex__8VIRasterRC7VIVect3 (x, y', z), then
//     EndTriFan__8VIRaster. SetZBias(raster, 0) and the matrix destructors follow; the result is 0.
// Inferred: a projected blob shadow for an actor onto nearby triangles. Unknown: member names, what the
// leading word of a triangle is, the node ids' meaning, and the VIActorFilter layout (the 40-byte size here
// is assumed from the original frame, where the copied words follow 16 bytes of the object).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void __13VIActorFilter(unsigned char* filter);
void _$_10VIMatrix44(unsigned char* matrix, int flags);
unsigned GetActorSpritePtr__C7VIScenei(unsigned char* scene, int actor);
void GetActorTransform__7VISceneiR10VIMatrix44(unsigned char* scene, int actor, unsigned char* matrix);
void Invert__10VIMatrix44RC10VIMatrix44(unsigned char* to, unsigned char* from);
void CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(unsigned sprite, int node, int actor, unsigned char* scene, unsigned char* matrix);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(unsigned char* matrix, unsigned char* in, unsigned char* out);
void Init__6VIBBoxiP7VIVect3(unsigned char* box, int count, unsigned char* points);
void Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(unsigned char* box, unsigned char* matrix, unsigned char* out);
unsigned char* GetActorLocation__C7VIScenei(unsigned char* scene, int actor);
void Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f(unsigned char* list, unsigned char* box, unsigned char* direction, float distance);
void QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList(unsigned char* scene, unsigned char* box, unsigned char* filter, unsigned char* list);
void SetMaterial__8VIRasteri(unsigned raster, int material);
void SetZBias__8VIRasteri(unsigned raster, int bias);
void BeginTriFan__8VIRaster(unsigned raster);
void Color__8VIRasterRC10VIColor32F(unsigned raster, const float* color);
void UV__8VIRasterRC7VIVect2(unsigned raster, const float* uv);
void Vertex__8VIRasterRC7VIVect3(unsigned raster, const float* position);
void EndTriFan__8VIRaster(unsigned raster);

static inline float K(unsigned bits) { union { unsigned u; float f; } v; v.u = bits; return v.f; }
static inline float& F(unsigned char* base, unsigned offset) { return *reinterpret_cast<float*>(base + offset); }
static inline unsigned& W(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }

static void release_matrices(unsigned char* m2, unsigned char* m1, unsigned char* m0) {
    _$_10VIMatrix44(m2, 2);
    _$_10VIMatrix44(m1, 2);
    _$_10VIMatrix44(m0, 2);
}

int RenderShadow__7VIScenei(unsigned char* self, int actor) {
    alignas(16) float m0[16] = {0}, m1[16] = {0}, m2[16] = {0};
    unsigned char* M0 = reinterpret_cast<unsigned char*>(m0);
    unsigned char* M1 = reinterpret_cast<unsigned char*>(m1);
    unsigned char* M2 = reinterpret_cast<unsigned char*>(m2);
    alignas(16) float location[3] = {0.0f, 0.0f, 0.0f};
    alignas(16) unsigned char filter[48];
    __13VIActorFilter(filter);
    static const int nodes[6] = {5, 6, 4, 7, 0, 1};
    for (int i = 0; i < 6; ++i) *reinterpret_cast<int*>(filter + 16 + 4 * i) = nodes[i];
    if (static_cast<int>(W(self, 6272)) == -1) {
        release_matrices(M2, M1, M0);
        return -1;
    }
    alignas(16) float corners[18] = {0};
    alignas(16) float box[6] = {0};
    const unsigned sprite = GetActorSpritePtr__C7VIScenei(self, actor);
    if (sprite == 0) {
        release_matrices(M2, M1, M0);
        return -1;
    }
    GetActorTransform__7VISceneiR10VIMatrix44(self, actor, M0);
    Invert__10VIMatrix44RC10VIMatrix44(M1, M0);
    for (int i = 0; i < 6; ++i) {
        CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(sprite, *reinterpret_cast<const int*>(filter + 16 + 4 * i), actor, self, M2);
        location[0] = m2[12];
        location[1] = m2[13];
        location[2] = m2[14];
        Mul__C10VIMatrix44RC7VIVect3R7VIVect3(M1, reinterpret_cast<unsigned char*>(location), reinterpret_cast<unsigned char*>(corners + 3 * i));
        if (i >= 4) {
            corners[3 * i + 0] = corners[3 * i + 0] * K(0x3f000000);
            corners[3 * i + 1] = corners[3 * i + 1] * K(0x3f000000);
            corners[3 * i + 2] = corners[3 * i + 2] * K(0x3f000000);
        }
    }
    Init__6VIBBoxiP7VIVect3(reinterpret_cast<unsigned char*>(box), 6, reinterpret_cast<unsigned char*>(corners));
    box[0] = box[0] - K(0x3e4ccccc);
    box[1] = box[1] - 0.0f;
    box[2] = box[2] - K(0x3e4ccccc);
    box[3] = box[3] + K(0x3e4ccccc);
    box[4] = box[4] + 0.0f;
    box[5] = box[5] + K(0x3e4ccccc);
    alignas(16) float region[6] = {0};
    Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(reinterpret_cast<unsigned char*>(box), M0, reinterpret_cast<unsigned char*>(region));
    const float* where = reinterpret_cast<const float*>(GetActorLocation__C7VIScenei(self, actor));
    location[0] = where[0];
    location[1] = where[1];
    region[4] = where[1] + K(0x3f000000);
    region[1] = where[1] - K(0x3f800000);
    location[2] = where[2];
    alignas(16) float up[3] = {0.0f, K(0x3f800000), 0.0f};
    Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f(self + 6128, reinterpret_cast<unsigned char*>(region), reinterpret_cast<unsigned char*>(up), K(0x42a00000));
    QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList(self, reinterpret_cast<unsigned char*>(region), filter, self + 6128);
    const unsigned raster = W(self, 19356);
    SetMaterial__8VIRasteri(raster, static_cast<int>(W(self, 6272)));
    SetZBias__8VIRasteri(raster, 15);
    unsigned char* list = self + 6128;
    if (static_cast<int>(W(list, 0)) > 0) {
        for (int i = 0;;) {
            const unsigned char* tri = NORRATH_WORD_TO_POINTER(W(list, 8)) + 40 * i;
            ++i;
            const float minx = box[0];
            const float inv_dx = K(0x3f800000) / (box[3] - minx);
            const float minz = box[2];
            const float inv_dz = K(0x3f800000) / (box[5] - minz);
            const float locy = location[1];
            float px[3], py[3], pz[3], pu[3], pv[3], pa[3];
            for (int k = 0; k < 3; ++k) {
                px[k] = *reinterpret_cast<const float*>(tri + 4 + 12 * k);
                py[k] = *reinterpret_cast<const float*>(tri + 8 + 12 * k) + K(0x3d23d70a);
                pz[k] = *reinterpret_cast<const float*>(tri + 12 + 12 * k);
                pu[k] = (((((px[k] * m1[0]) + (py[k] * m1[4])) + (pz[k] * m1[8])) + m1[12]) - minx) * inv_dx;
                pv[k] = (((((px[k] * m1[2]) + (py[k] * m1[6])) + (pz[k] * m1[10])) + m1[14]) - minz) * inv_dz;
                float a = locy - py[k];
                if (a < 0.0f) a = 0.0f;
                if (a > K(0x3f800000)) a = K(0x3f800000);
                pa[k] = a;
            }
            BeginTriFan__8VIRaster(raster);
            for (int k = 0; k < 3; ++k) {
                const float color[4] = {K(0x3f800000), K(0x3f800000), K(0x3f800000), (K(0x3f800000) - pa[k]) * K(0x3f266666)};
                const float uv[2] = {pu[k], pv[k]};
                const float position[3] = {px[k], py[k], pz[k]};
                Color__8VIRasterRC10VIColor32F(raster, color);
                UV__8VIRasterRC7VIVect2(raster, uv);
                Vertex__8VIRasterRC7VIVect3(raster, position);
            }
            EndTriFan__8VIRaster(raster);
            if (!(i < static_cast<int>(W(list, 0)))) break;
        }
    }
    SetZBias__8VIRasteri(raster, 0);
    release_matrices(M2, M1, M0);
    return 0;
}
}
