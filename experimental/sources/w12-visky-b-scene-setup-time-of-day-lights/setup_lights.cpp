// Observed: SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend at 0x01140250,
// 3056 bytes. Arguments: the scene, the camera and a profile blend (four profile indices at
// +0..+12, four float weights at +16..+28). The result is 0, or -1 when the emitter query fails.
// Offsets are bytes. The raster is the pointer stored at scene+19356 (read again before every
// raster call); the atmosphere is scene+1600; the camera matrix is camera+48, its position
// camera+28. A local multimap of emitter entries (constructed first, destroyed last with flags 2)
// follows a 16-byte query object.
//   Setup. overcast = GetOvercastInterp(atmosphere); time = float at scene+8160;
//   hour = (int)time, next = hour+1 (0 when that is 24), frac = time - (float)hour. Hour records
//   are 108 bytes apart. lerp(off) means a four-float vector made as
//   (record[next]+off - record[hour]+off) * frac + record[hour]+off, per component.
//   L = overcast * ((((f0*w0 + f1*w1) + f2*w2) + f3*w3)) where f_k is the float at
//   scene + 120 + 40 * profile[k].
//   1. P = lerp(8180), Q = lerp(8196), R = ((Q - P) * L) + P: SetAmbient(raster, R) then
//      EnableAmbient(raster, 1).
//   2. P = lerp(8224), Q = lerp(8240), S = ((Q - P) * L) + P. The sun direction D (three floats) is
//      lerp(8212) normalised with Normalize__7VIVect3.
//   3. If GetLightning(atmosphere, &x) is nonzero with x the amount, and t = scene+6848 (float):
//      S.r += ((1 - S.r) * t) * (x * 0.9f), the same for S.g, and S.b += ((1 - S.b) * x) * t (no
//      0.9 factor on the third channel in the image), and D becomes the three floats at scene+9292.
//   4. SetSunLight(raster, camera+48, {D, S}) (28 bytes: D then S), EnableSunLight(raster, 1); the 16
//      bytes of S are stored at scene+8164; EnableAllDirLights(raster, 0).
//   5. n = MaxPointLights(raster); with n <= 0 nothing further happens (result 0, map destroyed).
//      Otherwise EnablePointLight(raster, i, 0) for i = 0 .. MaxPointLights(raster)-1, then
//      EnableTessellation(raster, 0).
//   6. state = the unsigned word at scene+10772. If state < 2: step = (float)(d) * 0x3b03126e where d
//      is the 64-bit at scene+1152 minus the 64-bit at scene+10808, or 0 when d <= 0. The float
//      at scene+10816 grows by step when state == 1, time < 6, time > 16, or (float at scene+1672 > 0
//      and the float at scene+6848 > 0x3d4ccccc and overcast > 0.5); otherwise it shrinks by step.
//      It is stored, bounded below by 0 and above by the float at scene+10788 (min.s/max.s), and
//      stored again; when the bounded value v is > 0 a point light (position = floats at
//      scene+10776, then v, then the 16 bytes at scene+10792) is set with SetPointLight(raster, 0,
//      camera+48, &light), EnablePointLight(raster, 0, 1), EnableTessellation(raster, 1). In every
//      state < 2 case the 64-bit at scene+1152 is then stored at scene+10808.
//   7. If MaxPointLights(raster) < 2 skip to the end; else Init__14VIEmitterQueryi12VISpriteType(
//      query, Max-1, 8) and a query block {0, scene, camera, camera position, 500.0f, query} for
//      QueryEmitters__7VIWorldR23VISceneEmitterQueryVars(scene+4, &block); a negative result
//      destroys the map and gives -1. Otherwise for each map entry in order (112-byte entries at the
//      data pointer map+8; the word at +36 is a sprite index into the sprite pool at scene+996 whose
//      entries are 12 bytes) with light number j starting at 1: a point light with position 0, the float
//      at sprite+44, and the 16 bytes at sprite+64 is set with SetPointLight(raster, j, entry+48,
//      &light) and EnablePointLight(raster, j, 1).
//   8. The word at scene+1348 is incremented (also when MaxPointLights < 2), the map destroyed and 0
//      returned.
// Inferred: the scene keeps a table of 40-byte terrain profile records at +100 whose float at +20
// scales the day-cycle blend; the hour records carry ambient, sun and sun-direction vectors.
// Unknown: the meaning of the fields at +10772..+10816 and scene+6848, +1672 and +1348.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIScene;
struct VICamera;
struct VITerrainProfileBlend { int profile[4]; float weight[4]; };

void __t10VIMultiMap2ZfZ14VIEmitterEntry(void* map);
void _$_t10VIMultiMap2ZfZ14VIEmitterEntry(void* map, int flags);
float GetOvercastInterp__12VIAtmosphere(void* atmosphere);
int GetLightning__12VIAtmospherePf(void* atmosphere, float* amount);
void SetAmbient__8VIRasterRC10VIColor32F(void* raster, const float* color);
void EnableAmbient__8VIRasteri(void* raster, int enable);
void Normalize__7VIVect3(float* vector);
void SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight(void* raster, const void* matrix, const void* light);
void EnableSunLight__8VIRasteri(void* raster, int enable);
void EnableAllDirLights__8VIRasteri(void* raster, int enable);
int MaxPointLights__8VIRaster(void* raster);
void EnablePointLight__8VIRasterii(void* raster, int index, int enable);
void EnableTessellation__8VIRasteri(void* raster, int enable);
void SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight(void* raster, int index, const void* matrix, const void* light);
void Init__14VIEmitterQueryi12VISpriteType(void* query, int capacity, int type);
int QueryEmitters__7VIWorldR23VISceneEmitterQueryVars(void* world, void* vars);
int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void* map);
int End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void* map);
int Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(const void* map, int index);

static float lower_bound(float a, float b) {
    float result;
#if defined(__mips__)
    __asm__ volatile ("max.s %0, %1, %2" : "=f" (result) : "f" (a), "f" (b));
#else
    result = a > b ? a : b;
#endif
    return result;
}
static float upper_bound(float a, float b) {
    float result;
#if defined(__mips__)
    __asm__ volatile ("min.s %0, %1, %2" : "=f" (result) : "f" (a), "f" (b));
#else
    result = a < b ? a : b;
#endif
    return result;
}

static unsigned word_at(const char* address) { return *reinterpret_cast<const unsigned*>(address); }
static float float_at(const char* address) { return *reinterpret_cast<const float*>(address); }
static void* raster_of(const char* scene) { return NORRATH_TRANSLATE(word_at(scene + 19356)); }

// out = (next - hour) * frac + hour, one component at a time.
static void lerp4(float* out, const float* hour, const float* next, float frac) {
    for (int i = 0; i < 4; ++i) out[i] = next[i];
    for (int i = 0; i < 4; ++i) out[i] = out[i] - hour[i];
    for (int i = 0; i < 4; ++i) out[i] = out[i] * frac;
    for (int i = 0; i < 4; ++i) out[i] = out[i] + hour[i];
}

// out = (q - p) * l + p.
static void blend4(float* out, const float* q, const float* p, float l) {
    for (int i = 0; i < 4; ++i) out[i] = q[i];
    for (int i = 0; i < 4; ++i) out[i] = out[i] - p[i];
    for (int i = 0; i < 4; ++i) out[i] = out[i] * l;
    for (int i = 0; i < 4; ++i) out[i] = out[i] + p[i];
}

struct DirLight { float direction[3]; float color[4]; };
struct PointLight { float position[3]; float intensity; float color[4]; };
struct QueryBlock { unsigned zero; unsigned scene; unsigned camera; float position[3]; float radius; unsigned query; };

struct Locals {
    char query[16];
    char map[32];
};

int SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend(VIScene* scene_, VICamera* camera_, VITerrainProfileBlend* blend) {
    char* scene = reinterpret_cast<char*>(scene_);
    const char* camera = reinterpret_cast<const char*>(camera_);
    Locals locals;
    char* query = locals.query;
    char* map = locals.map;
    __t10VIMultiMap2ZfZ14VIEmitterEntry(map);

    void* atmosphere = scene + 1600;
    float overcast = GetOvercastInterp__12VIAtmosphere(atmosphere);
    float time = float_at(scene + 8160);
    int hour = static_cast<int>(time);
    int next = hour + 1;
    if (next == 24) next = 0;
    const char* hour_record = scene + 108 * hour;
    const char* next_record = scene + 108 * next;

    float field0 = float_at(scene + 120 + 40 * blend->profile[0]) * blend->weight[0];
    float field1 = float_at(scene + 120 + 40 * blend->profile[1]) * blend->weight[1];
    float field2 = float_at(scene + 120 + 40 * blend->profile[2]) * blend->weight[2];
    float field3 = float_at(scene + 120 + 40 * blend->profile[3]) * blend->weight[3];
    float level = field0;
    level = level + field1;
    level = level + field2;
    level = level + field3;
    level = overcast * level;
    float frac = time - static_cast<float>(hour);

    float p[4], q[4], r[4];
    lerp4(p, reinterpret_cast<const float*>(hour_record + 8180), reinterpret_cast<const float*>(next_record + 8180), frac);
    lerp4(q, reinterpret_cast<const float*>(hour_record + 8196), reinterpret_cast<const float*>(next_record + 8196), frac);
    blend4(r, q, p, level);
    SetAmbient__8VIRasterRC10VIColor32F(raster_of(scene), r);
    EnableAmbient__8VIRasteri(raster_of(scene), 1);

    DirLight sun;
    lerp4(p, reinterpret_cast<const float*>(hour_record + 8224), reinterpret_cast<const float*>(next_record + 8224), frac);
    lerp4(q, reinterpret_cast<const float*>(hour_record + 8240), reinterpret_cast<const float*>(next_record + 8240), frac);
    blend4(sun.color, q, p, level);
    {
        const float* from = reinterpret_cast<const float*>(hour_record + 8212);
        const float* to = reinterpret_cast<const float*>(next_record + 8212);
        for (int i = 0; i < 3; ++i) sun.direction[i] = to[i];
        for (int i = 0; i < 3; ++i) sun.direction[i] = sun.direction[i] - from[i];
        for (int i = 0; i < 3; ++i) sun.direction[i] = sun.direction[i] * frac;
        for (int i = 0; i < 3; ++i) sun.direction[i] = sun.direction[i] + from[i];
    }
    Normalize__7VIVect3(sun.direction);

    float amount;
    if (GetLightning__12VIAtmospherePf(atmosphere, &amount)) {
        float flash[4] = {sun.color[0], sun.color[1], sun.color[2], sun.color[3]};
        float t = float_at(scene + 6848);
        float scaled = amount * __builtin_bit_cast(float, 0x3f666666u);
        float add_r = ((1.0f - flash[0]) * t) * scaled;
        float add_b = ((1.0f - flash[2]) * amount) * t;
        float add_g = ((1.0f - flash[1]) * t) * scaled;
        flash[0] = flash[0] + add_r;
        flash[2] = flash[2] + add_b;
        flash[1] = flash[1] + add_g;
        for (int i = 0; i < 4; ++i) sun.color[i] = flash[i];
        sun.direction[0] = float_at(scene + 9292);
        sun.direction[1] = float_at(scene + 9296);
        sun.direction[2] = float_at(scene + 9300);
    }

    const char* camera_matrix = camera + 48;
    SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight(raster_of(scene), camera_matrix, &sun);
    EnableSunLight__8VIRasteri(raster_of(scene), 1);
    for (int i = 0; i < 4; ++i) reinterpret_cast<float*>(scene + 8164)[i] = sun.color[i];
    EnableAllDirLights__8VIRasteri(raster_of(scene), 0);

    if (MaxPointLights__8VIRaster(raster_of(scene)) <= 0) {
        _$_t10VIMultiMap2ZfZ14VIEmitterEntry(map, 2);
        return 0;
    }
    for (int i = 0; i < MaxPointLights__8VIRaster(raster_of(scene)); ++i)
        EnablePointLight__8VIRasterii(raster_of(scene), i, 0);
    EnableTessellation__8VIRasteri(raster_of(scene), 0);

    unsigned state = word_at(scene + 10772);
    if (state < 2) {
        long long stamp = *reinterpret_cast<long long*>(scene + 1152);
        long long elapsed = stamp - *reinterpret_cast<long long*>(scene + 10808);
        float step;
        if (elapsed > 0) step = static_cast<float>(elapsed) * __builtin_bit_cast(float, 0x3b03126eu);
        else step = 0.0f;
        float value = float_at(scene + 10816);
        bool grow;
        if (state == 1) grow = true;
        else if (time < 6.0f) grow = true;
        else if (16.0f < time) grow = true;
        else if (0.0f < float_at(scene + 1672) && __builtin_bit_cast(float, 0x3d4cccccu) < float_at(scene + 6848) && 0.5f < overcast) grow = true;
        else grow = false;
        if (grow) value = value + step;
        else value = value - step;
        *reinterpret_cast<float*>(scene + 10816) = value;
        float bounded = upper_bound(lower_bound(float_at(scene + 10816), 0.0f), float_at(scene + 10788));
        *reinterpret_cast<float*>(scene + 10816) = bounded;
        if (0.0f < bounded) {
            PointLight light;
            light.position[0] = float_at(scene + 10776);
            light.position[1] = float_at(scene + 10780);
            light.position[2] = float_at(scene + 10784);
            light.intensity = bounded;
            for (int i = 0; i < 4; ++i) light.color[i] = float_at(scene + 10792 + 4 * i);
            SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight(raster_of(scene), 0, camera_matrix, &light);
            EnablePointLight__8VIRasterii(raster_of(scene), 0, 1);
            EnableTessellation__8VIRasteri(raster_of(scene), 1);
            stamp = *reinterpret_cast<long long*>(scene + 1152);
        }
        *reinterpret_cast<long long*>(scene + 10808) = stamp;
    }

    if (MaxPointLights__8VIRaster(raster_of(scene)) >= 2) {
        Init__14VIEmitterQueryi12VISpriteType(query, MaxPointLights__8VIRaster(raster_of(scene)) - 1, 8);
        QueryBlock block;
        block.zero = 0;
        block.scene = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(scene));
        block.camera = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(camera));
        block.position[0] = float_at(camera + 28);
        block.position[1] = float_at(camera + 32);
        block.position[2] = float_at(camera + 36);
        block.radius = 500.0f;
        block.query = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(query));
        if (QueryEmitters__7VIWorldR23VISceneEmitterQueryVars(scene + 4, &block) < 0) {
            _$_t10VIMultiMap2ZfZ14VIEmitterEntry(map, 2);
            return -1;
        }
        int light_number = 1;
        for (int i = Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map); i != End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map);
             i = Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(map, i)) {
            const char* entry = NORRATH_TRANSLATE(word_at(map + 8)) + 112 * i;
            PointLight light;
            light.position[0] = 0.0f;
            light.position[1] = 0.0f;
            light.position[2] = 0.0f;
            const char* pool = NORRATH_TRANSLATE(word_at(scene + 996 + 8));
            const char* sprite = NORRATH_TRANSLATE(word_at(pool + 12 * static_cast<int>(word_at(entry + 36))));
            light.intensity = float_at(sprite + 44);
            for (int c = 0; c < 4; ++c) light.color[c] = float_at(sprite + 64 + 4 * c);
            SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight(raster_of(scene), light_number, NORRATH_TRANSLATE(word_at(map + 8)) + 112 * i + 48, &light);
            EnablePointLight__8VIRasterii(raster_of(scene), light_number, 1);
            ++light_number;
        }
    }
    *reinterpret_cast<int*>(scene + 1348) = *reinterpret_cast<int*>(scene + 1348) + 1;
    _$_t10VIMultiMap2ZfZ14VIEmitterEntry(map, 2);
    return 0;
}
}
