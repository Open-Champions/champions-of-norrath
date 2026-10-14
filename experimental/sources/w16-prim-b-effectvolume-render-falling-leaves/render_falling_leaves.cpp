// Observed: RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44 at 0x01147e70 is 968
// bytes. Only the object (a0) is used; the camera and matrix arguments are ignored. Offsets are
// bytes into the object.
//   1. Returns 0 when the float at 92 equals 0.0, or when the float at 96 equals 0.0.
//   2. Reads the particle pointer at 88 and the scene pointer word _14VIEffectVolume$mScene
//      (0x011b1a98); the raster is the pointer at scene + 19356. Returns -1 when the particle
//      pointer is null.
//   3. count = (int)(float92 * (float)(int word at 84)).
//   4. With the material word m at 76: m == -1 calls SetSolidFillUIAVGMaterial__8VIRaster(raster),
//      anything else SetMaterial__8VIRasteri(raster, m).
//   5. Four local float triples are zeroed. If count <= 0 it returns 0. For each of the count
//      36-byte particles p (the pointer at 88 advancing by 36): when the word p[0] is zero the
//      particle is skipped; otherwise the 48-byte record at sFallEffect (0x011b0298) +
//      p.int[4] * 1440 + (int)p.float[8] * 48 supplies four vertex triples; each triple has the
//      particle's floats at 12, 16, 20 added in x, y, z. A colour of (1.0, 1.0, 1.0, p.float[32] *
//      float96) is set with Color__8VIRasterRC10VIColor32F, then BeginTriFan__8VIRaster, then six
//      pairs UV__8VIRasterRC7VIVect2 then Vertex__8VIRasterRC7VIVect3 on triples 0, 1, 2, 3, 2, 1
//      with texture coordinates (0,0), (1,0), (1,1), (0,1), (1,1), (1,0), then EndTriFan__8VIRaster.
//   6. Returns 0.
// Inferred: a billboard-free quad per falling leaf drawn as a fan that doubles back (the last two
// vertices repeat triples 2 and 1); the record table is [type][frame] of four corner offsets (the
// 2880-byte sFallEffect is 2 types of 30 frames); float96 scales the particle alpha and float92 the
// share of the particle array that is drawn. Unknown: the names of the object fields, the units of
// float92, the meaning of particle words 24 and 28, and the colour channel order of the Color call.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern unsigned _14VIEffectVolume$mScene;
extern unsigned char sFallEffect[];

extern "C" {
void SetMaterial__8VIRasteri(void* raster, int material);
void SetSolidFillUIAVGMaterial__8VIRaster(void* raster);
void Color__8VIRasterRC10VIColor32F(void* raster, const float* color);
void BeginTriFan__8VIRaster(void* raster);
void UV__8VIRasterRC7VIVect2(void* raster, const float* uv);
void Vertex__8VIRasterRC7VIVect3(void* raster, const float* vertex);
void EndTriFan__8VIRaster(void* raster);

int RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44(unsigned char* self, void*, void*) {
    if (*reinterpret_cast<float*>(self + 92) == 0.0f)
        return 0;
    if (*reinterpret_cast<float*>(self + 96) == 0.0f)
        return 0;
    unsigned char* particle = at(*reinterpret_cast<unsigned*>(self + 88));
    unsigned scene = _14VIEffectVolume$mScene;
    if (*reinterpret_cast<unsigned*>(self + 88) == 0)
        return -1;
    unsigned char* raster = at(*reinterpret_cast<unsigned*>(at(scene) + 19356));
    int count = static_cast<int>(*reinterpret_cast<float*>(self + 92) * static_cast<float>(*reinterpret_cast<int*>(self + 84)));
    int material = *reinterpret_cast<int*>(self + 76);
    if (material != -1)
        SetMaterial__8VIRasteri(raster, material);
    else
        SetSolidFillUIAVGMaterial__8VIRaster(raster);
    float corner[12];
    for (int i = 0; i < 12; ++i)
        corner[i] = 0.0f;
    if (count <= 0)
        return 0;
    for (int n = count; n != 0; --n, particle += 36) {
        if (*reinterpret_cast<int*>(particle) == 0)
            continue;
        int type = *reinterpret_cast<int*>(particle + 4);
        int frame = static_cast<int>(*reinterpret_cast<float*>(particle + 8));
        const float* record = reinterpret_cast<const float*>(sFallEffect + type * 1440 + frame * 48);
        for (int i = 0; i < 12; ++i)
            corner[i] = record[i];
        for (int c = 0; c < 4; ++c) {
            corner[3 * c] = corner[3 * c] + *reinterpret_cast<float*>(particle + 12);
            corner[3 * c + 1] = corner[3 * c + 1] + *reinterpret_cast<float*>(particle + 16);
            corner[3 * c + 2] = corner[3 * c + 2] + *reinterpret_cast<float*>(particle + 20);
        }
        float color[4];
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
        color[3] = *reinterpret_cast<float*>(particle + 32) * *reinterpret_cast<float*>(self + 96);
        Color__8VIRasterRC10VIColor32F(raster, color);
        BeginTriFan__8VIRaster(raster);
        static const int order[6] = {0, 1, 2, 3, 2, 1};
        static const float uv[6][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};
        for (int i = 0; i < 6; ++i) {
            float coordinate[2] = {uv[i][0], uv[i][1]};
            UV__8VIRasterRC7VIVect2(raster, coordinate);
            Vertex__8VIRasterRC7VIVect3(raster, corner + 3 * order[i]);
        }
        EndTriFan__8VIRaster(raster);
    }
    return 0;
}
}
