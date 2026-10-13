// Host check, not an EE input. Word-sized pointers inside the scene are mapped through a small
// table so the 4-byte fields can name host buffers.
#define NORRATH_HOST_CONTRACT 1
#include "setup_lights.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

alignas(8) static char scene[0x5000];
alignas(8) static char camera[0x100];
alignas(8) static char raster_buffer[16];
alignas(8) static char pool_data[8 * 12];
alignas(8) static char sprites[8][0x100];
alignas(8) static char entries[4][112];
static VITerrainProfileBlend blend;

struct Mapping { unsigned word; char* host; };
static Mapping mappings[32];
static int mapping_count;
static unsigned map_word(char* host) {
    for (int i = 0; i < mapping_count; ++i) if (mappings[i].host == host) return mappings[i].word;
    unsigned word = 0x10000u + 0x1000u * (mapping_count + 1);
    mappings[mapping_count++] = {word, host};
    return word;
}
extern "C" char* norrath_host_translate(unsigned address) {
    for (int i = 0; i < mapping_count; ++i) if (mappings[i].word == address) return mappings[i].host;
    return 0;
}

struct Event { int kind; long a, b, c; float f[8]; };
static std::vector<Event> events;
enum { MAPCTOR = 1, OVERCAST, AMBIENT, ENAMBIENT, NORMALIZE, SUN, ENSUN, ENALL, MAXP, ENPL, ENTESS, SETPL, QINIT, QUERY, MAPDTOR, LIGHTNING, BEGIN, END, NEXT };
static float overcast_value = 0.5f;
static int lightning_result;
static float lightning_amount;
static int maxp_value = 4;
static int query_result;
static int entry_count;
static char* last_map;

static void push(int kind, long a = 0, long b = 0, long c = 0, const float* f = 0, int n = 0) {
    Event e = {kind, a, b, c, {0}};
    for (int i = 0; i < n; ++i) e.f[i] = f[i];
    events.push_back(e);
}

extern "C" {
void __t10VIMultiMap2ZfZ14VIEmitterEntry(void* map) {
    last_map = static_cast<char*>(map);
    unsigned data = map_word(&entries[0][0]);
    std::memcpy(static_cast<char*>(map) + 8, &data, 4);
    push(MAPCTOR);
}
void _$_t10VIMultiMap2ZfZ14VIEmitterEntry(void*, int flags) { push(MAPDTOR, flags); }
float GetOvercastInterp__12VIAtmosphere(void* a) { push(OVERCAST, a == scene + 1600); return overcast_value; }
int GetLightning__12VIAtmospherePf(void* a, float* amount) { push(LIGHTNING, a == scene + 1600); *amount = lightning_amount; return lightning_result; }
void SetAmbient__8VIRasterRC10VIColor32F(void* r, const float* c) { push(AMBIENT, r == raster_buffer, 0, 0, c, 4); }
void EnableAmbient__8VIRasteri(void* r, int e) { push(ENAMBIENT, r == raster_buffer, e); }
void Normalize__7VIVect3(float* v) {
    push(NORMALIZE, 0, 0, 0, v, 3);
    float n = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    for (int i = 0; i < 3; ++i) v[i] /= n;
}
void SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight(void* r, const void* m, const void* light) {
    push(SUN, r == raster_buffer, m == camera + 48, 0, static_cast<const float*>(light), 7);
}
void EnableSunLight__8VIRasteri(void* r, int e) { push(ENSUN, r == raster_buffer, e); }
void EnableAllDirLights__8VIRasteri(void* r, int e) { push(ENALL, r == raster_buffer, e); }
int MaxPointLights__8VIRaster(void* r) { push(MAXP, r == raster_buffer); return maxp_value; }
void EnablePointLight__8VIRasterii(void* r, int i, int e) { push(ENPL, r == raster_buffer, i, e); }
void EnableTessellation__8VIRasteri(void* r, int e) { push(ENTESS, r == raster_buffer, e); }
void SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight(void* r, int i, const void* m, const void* light) {
    long which = m == camera + 48 ? -1 : (static_cast<const char*>(m) - &entries[0][0]);
    push(SETPL, r == raster_buffer, i, which, static_cast<const float*>(light), 8);
}
void Init__14VIEmitterQueryi12VISpriteType(void* q, int capacity, int type) { push(QINIT, q == last_map - 16, capacity, type); }
int QueryEmitters__7VIWorldR23VISceneEmitterQueryVars(void* world, void* block) {
    const unsigned* w = static_cast<const unsigned*>(block);
    float f[4];
    std::memcpy(f, w + 3, 16);
    push(QUERY, world == scene + 4, w[0], 0, f, 4);
    return query_result;
}
int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void*) { push(BEGIN); return 0; }
int End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void*) { push(END); return entry_count; }
int Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(const void*, int i) { push(NEXT, i); return i + 1; }
}

static void put(int offset, float v) { std::memcpy(scene + offset, &v, 4); }
static void put_word(int offset, unsigned v) { std::memcpy(scene + offset, &v, 4); }
static float get(int offset) { float v; std::memcpy(&v, scene + offset, 4); return v; }
static void put64(int offset, long long v) { std::memcpy(scene + offset, &v, 8); }
static int failures;
#define EXPECT(cond) do { if (!(cond) && !failures) { failures = __LINE__; std::fprintf(stderr, "check failed at line %d\n", __LINE__); } } while (0)

static void reset() {
    std::memset(scene, 0, sizeof scene);
    std::memset(camera, 0, sizeof camera);
    std::memset(entries, 0, sizeof entries);
    std::memset(sprites, 0, sizeof sprites);
    events.clear();
    mapping_count = 0;
    overcast_value = 0.5f;
    lightning_result = 0;
    lightning_amount = 0.0f;
    maxp_value = 4;
    query_result = 0;
    entry_count = 0;
    std::memset(&blend, 0, sizeof blend);
    put_word(19356, map_word(raster_buffer));
    put_word(996 + 8, map_word(pool_data));
    // Profile 0 only, weight 1, its +20 field is 1.0 so L = overcast.
    blend.weight[0] = 1.0f;
    put(120, 1.0f);
}

static int run() {
    return SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend(
        reinterpret_cast<VIScene*>(scene), reinterpret_cast<VICamera*>(camera), &blend);
}

static const Event* find(int kind, int nth = 0) {
    for (const Event& e : events) if (e.kind == kind && nth-- == 0) return &e;
    return 0;
}

int main() {
    // A: no point lights available (MaxPointLights <= 0).
    reset();
    maxp_value = 0;
    put(8160, 0.0f);
    for (int i = 0; i < 4; ++i) { put(8180 + 4 * i, 0.25f); put(8196 + 4 * i, 0.75f); put(8224 + 4 * i, 0.5f); put(8240 + 4 * i, 1.0f); }
    put(8212, 0.0f); put(8216, 3.0f); put(8220, 4.0f);
    put(8212 + 108, 0.0f); put(8216 + 108, 3.0f); put(8220 + 108, 4.0f);
    put_word(1348, 9);
    EXPECT(run() == 0);
    EXPECT(events.size() == 11);
    static const int expected_order[] = {MAPCTOR, OVERCAST, AMBIENT, ENAMBIENT, NORMALIZE, LIGHTNING, SUN, ENSUN, ENALL, MAXP, MAPDTOR};
    for (int i = 0; i < 11 && i < (int)events.size(); ++i) EXPECT(events[i].kind == expected_order[i]);
    // R = (Q - P) * L + P with P = .25, Q = .75, L = overcast .5 -> .5
    const Event* ambient = find(AMBIENT);
    EXPECT(ambient && ambient->a == 1);
    for (int i = 0; i < 4; ++i) EXPECT(ambient->f[i] == 0.5f);
    // S = (1.0 - .5) * .5 + .5 = .75 ; direction (0, 3, 4) normalised by the stub to (0, .6, .8).
    const Event* sun = find(SUN);
    EXPECT(sun && sun->a == 1 && sun->b == 1);
    EXPECT(sun->f[0] == 0.0f && std::fabs(sun->f[1] - 0.6f) < 1e-6f && std::fabs(sun->f[2] - 0.8f) < 1e-6f);
    for (int i = 3; i < 7; ++i) EXPECT(sun->f[i] == 0.75f);
    EXPECT(get(8164) == 0.75f && get(8176) == 0.75f);
    EXPECT(find(NORMALIZE)->f[1] == 3.0f);
    EXPECT(find(ENSUN)->b == 1 && find(ENALL)->b == 0 && find(ENAMBIENT)->b == 1);
    EXPECT(*reinterpret_cast<unsigned*>(scene + 1348) == 9);
    EXPECT(find(MAPDTOR)->a == 2);

    // B: interpolation by frac and the wrap of the hour after 23.
    reset();
    maxp_value = 0;
    put(8160, 23.5f);
    for (int i = 0; i < 4; ++i) { put(8180 + 108 * 23 + 4 * i, 0.0f); put(8180 + 4 * i, 1.0f); put(8196 + 108 * 23 + 4 * i, 0.0f); put(8196 + 4 * i, 1.0f); }
    overcast_value = 1.0f;
    put(8212 + 108 * 23, 1.0f); put(8212 + 108 * 23 + 4, 0.0f); put(8212 + 108 * 23 + 8, 0.0f);
    put(8212, 1.0f); put(8216, 0.0f); put(8220, 0.0f);
    run();
    // P = Q = 0.5 per component, R = (Q-P)*L+P = 0.5
    EXPECT(find(AMBIENT)->f[0] == 0.5f && find(AMBIENT)->f[3] == 0.5f);

    // C: lightning changes the colour and the direction.
    reset();
    maxp_value = 0;
    put(8160, 0.0f);
    for (int i = 0; i < 4; ++i) { put(8180 + 4 * i, 0.25f); put(8196 + 4 * i, 0.75f); put(8224 + 4 * i, 0.5f); put(8240 + 4 * i, 1.0f); }
    put(8212, 0.0f); put(8216, 3.0f); put(8220, 4.0f); put(8212 + 108, 0.0f); put(8216 + 108, 3.0f); put(8220 + 108, 4.0f);
    lightning_result = 1;
    lightning_amount = 0.5f;
    put(6848, 0.5f);
    put(9292, 1.0f); put(9296, 0.0f); put(9300, 0.0f);
    run();
    sun = find(SUN);
    {
        float t = 0.5f, amount = 0.5f, s = 0.75f;
        float scaled = amount * 0.9f;
        // 0.9f in the image is 0x3f666666 (just below the nearest float to 0.9).
        scaled = amount * __builtin_bit_cast(float, 0x3f666666u);
        float r = s + ((1.0f - s) * t) * scaled;
        float b = s + ((1.0f - s) * amount) * t;
        EXPECT(sun->f[3] == r && sun->f[4] == r && sun->f[5] == b && sun->f[6] == 0.75f);
        EXPECT(r != b);
        // The Normalize stub ran on the pre-lightning direction; the flash direction is passed unnormalised.
        EXPECT(sun->f[0] == 1.0f && sun->f[1] == 0.0f && sun->f[2] == 0.0f);
    }

    // D: point lights. MaxPointLights = 4, state 2: all disabled, tessellation off, one query, two entries.
    reset();
    maxp_value = 4;
    put_word(10772, 2);
    put(8160, 0.0f);
    put_word(1348, 9);

    {
        float pos[3] = {1.0f, 2.0f, 3.0f};
        std::memcpy(camera + 28, pos, 12);
    }
    entry_count = 2;
    for (int i = 0; i < 8; ++i) {
        unsigned w = map_word(sprites[i]);
        std::memcpy(pool_data + 12 * i, &w, 4);
        float inten = 0.25f * (i + 1);
        std::memcpy(sprites[i] + 44, &inten, 4);
        for (int c = 0; c < 4; ++c) { float v = 0.1f * (i + 1) + c; std::memcpy(sprites[i] + 64 + 4 * c, &v, 4); }
    }
    {
        unsigned idx0 = 5, idx1 = 2;
        std::memcpy(entries[0] + 36, &idx0, 4);
        std::memcpy(entries[1] + 36, &idx1, 4);
    }
    EXPECT(run() == 0);
    int disable_count = 0;
    for (const Event& e : events) if (e.kind == ENPL && e.c == 0) ++disable_count;
    EXPECT(disable_count == 4);
    EXPECT(find(ENTESS)->b == 0);
    const Event* init = find(QINIT);
    EXPECT(init && init->a == 1 && init->b == 3 && init->c == 8);
    const Event* query = find(QUERY);
    EXPECT(query && query->a == 1 && query->b == 0);
    EXPECT(query->f[0] == 1.0f && query->f[1] == 2.0f && query->f[2] == 3.0f && query->f[3] == 500.0f);
    const Event* light1 = find(SETPL, 0);
    const Event* light2 = find(SETPL, 1);
    EXPECT(light1 && light2 && !find(SETPL, 2));
    EXPECT(light1->b == 1 && light1->c == 48 && light2->b == 2 && light2->c == 112 + 48);
    EXPECT(light1->f[0] == 0.0f && light1->f[1] == 0.0f && light1->f[2] == 0.0f);
    EXPECT(light1->f[3] == 0.25f * 6);
    EXPECT(light1->f[4] == 0.1f * 6 + 0 && light1->f[7] == 0.1f * 6 + 3);
    EXPECT(light2->f[3] == 0.25f * 3);
    int enable_count = 0;
    for (const Event& e : events) if (e.kind == ENPL && e.c == 1) ++enable_count;
    EXPECT(enable_count == 2);
    EXPECT(*reinterpret_cast<unsigned*>(scene + 1348) == 10);
    EXPECT(events.back().kind == MAPDTOR);

    // E: query failure gives -1 without touching the counter.
    reset();
    put_word(10772, 2);
    put_word(1348, 9);
    query_result = -1;
    EXPECT(run() == -1);
    EXPECT(*reinterpret_cast<unsigned*>(scene + 1348) == 9);
    EXPECT(!find(SETPL));
    EXPECT(events.back().kind == MAPDTOR);

    // F: MaxPointLights = 1 skips the emitter query but still counts.
    reset();
    maxp_value = 1;
    put_word(10772, 2);
    put_word(1348, 9);
    EXPECT(run() == 0);
    EXPECT(!find(QUERY) && !find(QINIT));
    EXPECT(*reinterpret_cast<unsigned*>(scene + 1348) == 10);

    // G: state 0 with a positive time step grows the intensity at night and lights point light 0.
    reset();
    maxp_value = 1;
    put_word(10772, 0);
    put(8160, 3.0f);
    put64(1152, 1000 + 50);
    put64(10808, 1000);
    put(10816, 0.25f);
    put(10788, 0.75f);
    put(10776, 4.0f); put(10780, 5.0f); put(10784, 6.0f);
    for (int c = 0; c < 4; ++c) put(10792 + 4 * c, 0.5f + c);
    run();
    {
        float step = 50.0f * __builtin_bit_cast(float, 0x3b03126eu);
        float want = 0.25f + step;
        EXPECT(get(10816) == want);
        const Event* pl = find(SETPL);
        EXPECT(pl && pl->b == 0 && pl->c == -1);
        EXPECT(pl->f[0] == 4.0f && pl->f[1] == 5.0f && pl->f[2] == 6.0f && pl->f[3] == want);
        EXPECT(pl->f[4] == 0.5f && pl->f[7] == 3.5f);
        const Event* en = find(ENPL, 0);
        EXPECT(en && en->b == 0 && en->c == 0);
        EXPECT(find(ENPL, 1) && find(ENPL, 1)->c == 1);
        EXPECT(find(ENTESS, 1) && find(ENTESS, 1)->b == 1);
        long long stored;
        std::memcpy(&stored, scene + 10808, 8);
        EXPECT(stored == 1050);
    }
    // H: daytime without lightning storm conditions shrinks it, clamped at zero, no light set.
    reset();
    maxp_value = 1;
    put_word(10772, 0);
    put(8160, 10.0f);
    put64(1152, 3000); put64(10808, 1000);
    put(10816, 0.0005f);
    put(10788, 0.75f);
    run();
    EXPECT(get(10816) == 0.0f);
    EXPECT(!find(SETPL));
    long long stamp;
    std::memcpy(&stamp, scene + 10808, 8);
    EXPECT(stamp == 3000);
    // I: state 1 always grows, clamped at the maximum.
    reset();
    maxp_value = 1;
    put_word(10772, 1);
    put(8160, 10.0f);
    put64(1152, 1000000); put64(10808, 0);
    put(10816, 0.5f);
    put(10788, 0.75f);
    run();
    EXPECT(get(10816) == 0.75f);
    EXPECT(find(SETPL) != 0);
    // J: elapsed <= 0 means no change.
    reset();
    maxp_value = 1;
    put_word(10772, 1);
    put64(1152, 100); put64(10808, 200);
    put(10816, 0.5f); put(10788, 0.75f);
    run();
    EXPECT(get(10816) == 0.5f);
    return failures != 0;
}
