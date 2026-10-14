#include "render.cpp"
#include <string.h>

static int events[64]; static int event_n;
static int capacity = 2; static int begin_counts[8]; static int begin_n;
static unsigned char batches[8][4 * 48];
static int materials[4]; static int material_n;
static int removed_n; static void* removed[8]; static void* appended[8]; static void* append_list;
static float normalized_in[3];

extern "C" void SetMaterial__8VIRasteri(void*, int m) { materials[material_n++] = m; events[event_n++] = 1; }
extern "C" int BeginBillboards__8VIRasterPP11VIBillboardi(void*, unsigned char** cursor, int count) {
    begin_counts[begin_n] = count; *cursor = batches[begin_n++]; events[event_n++] = 2;
    return capacity < count ? capacity : count;
}
extern "C" void EndBillboards__8VIRaster(void*) { events[event_n++] = 3; }
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void*, const void* in, void* out) {
    const float* a = static_cast<const float*>(in); float* o = static_cast<float*>(out);
    o[0] = a[0] + 100.0f; o[1] = a[1] + 100.0f; o[2] = a[2] + 100.0f; events[event_n++] = 4;
}
extern "C" int Normalize__7VIVect3(void* v) {
    float* f = static_cast<float*>(v); memcpy(normalized_in, f, 12);
    f[0] *= 0.5f; f[1] *= 0.5f; f[2] *= 0.5f; events[event_n++] = 5; return 1;
}
extern "C" void Remove__7VILNode(void* node) { removed[removed_n] = node; events[event_n++] = 6; }
extern "C" void AddTail__7VILListP7VILNode(void* list, void* node) { append_list = list; appended[removed_n++] = node; events[event_n++] = 7; }

static void putf(unsigned char* b, int o, float v) { memcpy(b + o, &v, 4); }
static void putw(unsigned char* b, int o, unsigned int v) { memcpy(b + o, &v, 4); }
static float getf(const unsigned char* b, int o) { float v; memcpy(&v, b + o, 4); return v; }
static unsigned int getw(const unsigned char* b, int o) { unsigned int v; memcpy(&v, b + o, 4); return v; }
static unsigned int addr(const void* p) { return static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

int main() {
    static unsigned char system[128], sentinel_emitter[1024], emitter[1024], material[800];
    static unsigned char particles[4][112];
    static unsigned char raster[4];
    // history of frame times: eight entries at 20..48, previous time 1000
    for (int i = 0; i < 8; ++i) putf(system, 20 + 4 * i, 0.01f * (i + 1));
    long long previous = 1000; memcpy(system + 8, &previous, 8);
    putw(system, 52, addr(emitter)); putw(emitter, 0, addr(sentinel_emitter));
    putw(emitter, 84, addr(material)); putw(material, 776, 0x5150);
    putw(emitter, 172, 3);
    putw(emitter, 160, addr(particles[0]));
    for (int i = 0; i < 3; ++i) putw(particles[i], 0, addr(particles[i + 1]));
    // color table entries 7, 30 and 15 (16 bytes each from emitter+244)
    static const int slots[3] = {7, 30, 15};
    for (int i = 0; i < 3; ++i)
        for (int k = 0; k < 16; ++k) emitter[244 + 16 * slots[i] + k] = static_cast<unsigned char>(0x10 * (i + 1) + k);
    const float age[3] = {0.25f, 0.99f, 0.5f};
    const float rate[3] = {0.0f, 1.0f, 0.0f};
    const float drag[3] = {0.0f, 0.0f, 0.5f};
    for (int i = 0; i < 3; ++i) {
        unsigned char* p = particles[i];
        putf(p, 16, age[i]); putf(p, 20, rate[i]); putf(p, 24, drag[i]); putf(p, 28, 1.0f);
        putf(p, 32, 1.0f * i); putf(p, 36, 2.0f * i); putf(p, 40, -1.0f * i);
        putf(p, 44, 10.0f); putf(p, 48, 20.0f); putf(p, 52, 30.0f);
        putf(p, 88, 3.0f); putf(p, 92, 0.5f); putf(p, 96, -4.0f); putw(p, 100, 7 + i);
    }
    putw(emitter, 984, 1); putf(emitter, 964, 100.0f); putf(emitter, 968, 100.0f); putf(emitter, 972, 100.0f); putf(emitter, 976, 2.0f);

    int r = Render__16VIParticleSystemlR8VIRaster(system, 1050, raster);
    if (r != 0) return 1;
    long long stored; memcpy(&stored, system + 8, 8);
    if (stored != 1050) return 2;
    float dt = 50.0f * float_from_bits(0x3a83126eu);
    float sum = 0.0f; for (int i = 0; i < 7; ++i) sum += 0.01f * (i + 1);
    sum += dt;
    float step = sum * 0.125f;
    if (getf(system, 16) != step) return 3;
    if (getf(system, 20) != dt) return 4;
    for (int i = 1; i < 7; ++i) if (getf(system, 24 + 4 * i) != 0.01f * (i + 1)) return 5;
    if (getf(system, 24) != 0.01f) return 5;
    if (material_n != 1 || materials[0] != 0x5150) return 6;
    // mat, begin, p0 {mul, norm}, p1 {mul, end, norm, remove, addtail}, p2 {begin, mul, end, norm}
    static const int expect[] = {1, 2, 4, 5, 4, 3, 5, 6, 7, 2, 4, 3, 5};
    if (event_n != 13) return 7;
    for (int i = 0; i < 13; ++i) if (events[i] != expect[i]) return 7;
    if (begin_n != 2 || begin_counts[0] != 3 || begin_counts[1] != 1) return 8;
    // entries: position + 100 (the stubbed transform), float88 twice, word100 << 2, color row
    if (getf(batches[0], 0) != 100.0f || getf(batches[0] + 48, 0) != 101.0f || getf(batches[1], 0) != 102.0f) return 9;
    for (int i = 0; i < 3; ++i) {
        unsigned char* e = i == 2 ? batches[1] : batches[0] + 48 * i;
        if (getf(e, 16) != 3.0f || getf(e, 20) != 3.0f || getw(e, 12) != static_cast<unsigned>((7 + i) << 2)) return 10;
        for (int k = 0; k < 16; ++k) if (e[32 + k] != 0x10 * (i + 1) + k) return 11;
    }
    // positions advance by velocity * step, using the velocity before drag
    float pos[3][3];
    for (int i = 0; i < 3; ++i) {
        const float start[3] = {1.0f * i, 2.0f * i, -1.0f * i}, vel[3] = {10.0f, 20.0f, 30.0f};
        for (int k = 0; k < 3; ++k) {
            pos[i][k] = start[k] + vel[k] * step;
            if (getf(particles[i], 32 + 4 * k) != pos[i][k]) return 12;
        }
    }
    // the bounding box starts at +-1e7 and takes min and max over the new positions
    for (int k = 0; k < 3; ++k) {
        float lo = pos[0][k], hi = pos[0][k];
        for (int i = 1; i < 3; ++i) { if (pos[i][k] < lo) lo = pos[i][k]; if (pos[i][k] > hi) hi = pos[i][k]; }
        if (getf(emitter, 940 + 4 * k) != lo || getf(emitter, 952 + 4 * k) != hi) return 13;
    }
    // attractor: normalized input is target - position (last call: particle 2); velocity gains half of it * (2 * step)
    for (int k = 0; k < 3; ++k) {
        if (normalized_in[k] != 100.0f - pos[2][k]) return 14;
        float v = (10.0f + 10.0f * k) + 0.0f;
        float drag_factor = 1.0f - 0.5f * step;
        float expected = (v * drag_factor) + ((100.0f - pos[2][k]) * 0.5f) * (2.0f * step);
        if (k == 1) expected = expected + -4.0f * step;
        if (getf(particles[2], 44 + 4 * k) != expected) return 15;
    }
    // integration of the per-particle rates (the y velocity gains float96 * step after the attractor)
    if (getf(particles[0], 16) != 0.25f + 0.0f * step || getf(particles[1], 16) != 0.99f + 1.0f * step) return 16;
    if (getf(particles[2], 88) != 3.0f + 0.5f * step) return 17;
    // particle 1 reached age 1: removed, counted down and appended to the list at this+64
    if (removed_n != 1 || removed[0] != particles[1] || appended[0] != particles[1] || append_list != system + 64) return 18;
    if (getw(emitter, 172) != 2) return 19;
    // an empty particle list draws nothing but the frame time is still taken
    event_n = 0; material_n = 0;
    putw(emitter, 160, addr(particles[3]));
    putw(particles[3], 0, 0);
    Render__16VIParticleSystemlR8VIRaster(system, 1100, raster);
    if (event_n != 0 || material_n != 0) return 20;
    memcpy(&stored, system + 8, 8);
    if (stored != 1100) return 21;
    // a head with no following emitter does nothing
    putw(system, 52, addr(sentinel_emitter));
    putw(sentinel_emitter, 0, 0);
    Render__16VIParticleSystemlR8VIRaster(system, 1300000, raster);
    if (event_n != 0) return 22;
    // elapsed time is limited to the bits 0x3dcccccc
    unsigned int bits = 0; float clamp = getf(system, 20); memcpy(&bits, &clamp, 4);
    if (bits != 0x3dccccccu) return 23;
    return 0;
}
