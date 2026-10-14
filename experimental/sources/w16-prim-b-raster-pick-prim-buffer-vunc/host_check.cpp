// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "pick_prim_buffer.cpp"
#include <initializer_list>
#include <string.h>
#include <vector>

alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

struct Call {
    int kind;  // 0 cull, 1 pick, 2 hit color
    int material;
    float v[9];
    unsigned char c[12];
    const void* box;
};
static std::vector<Call> calls;
static std::vector<int> cullAnswers, pickAnswers;
static size_t cullAt, pickAt;

extern "C" int Cull__8VIRasterR13VIPickSegmentR6VIBBox(void*, void*, const void* box) {
    Call c{};
    c.kind = 0;
    c.box = box;
    calls.push_back(c);
    return cullAnswers[cullAt++ % cullAnswers.size()];
}
extern "C" int Pick__8VIRasterR13VIPickSegmentiR7VIVect3N23(void*, void*, int material, const float* a, const float* b, const float* c) {
    Call k{};
    k.kind = 1;
    k.material = material;
    memcpy(k.v, a, 12);
    memcpy(k.v + 3, b, 12);
    memcpy(k.v + 6, c, 12);
    calls.push_back(k);
    return pickAnswers[pickAt++ % pickAnswers.size()];
}
extern "C" void CalcHitColor__8VIRasterR13VIPickSegmentR7VIVect3N22R9VIColor32N25(void*, void*, const float* a, const float* b, const float* c,
                                                                                   const unsigned char* ca, const unsigned char* cb, const unsigned char* cc) {
    Call k{};
    k.kind = 2;
    memcpy(k.v, a, 12);
    memcpy(k.v + 3, b, 12);
    memcpy(k.v + 6, c, 12);
    memcpy(k.c, ca, 4);
    memcpy(k.c + 4, cb, 4);
    memcpy(k.c + 8, cc, 4);
    calls.push_back(k);
}

static void put32(unsigned char* p, unsigned v) { memcpy(p, &v, 4); }

int main() {
    // pb at 0x7000; groups at 0x100; entries at 0x200; slots at 0x400; palette at 0x6800.
    unsigned char* pb = arena + 0x7000;
    unsigned char* pal = arena + 0x6800;
    // two groups: material 1 with 2 entries, material 0 with 1 entry; vertex counts 4, 2, 3
    int counts[3] = {4, 2, 3};
    for (int variant = 0; variant < 3; ++variant) {
        memset(arena, 0, sizeof arena);
        put32(pb + 64, 2);
        put32(pb + 88, 0x100);
        put32(pb + 92, 0x200);
        put32(pb + 96, 0x400);
        put32(pal + 12, 0x6c00);
        put32(arena + 0x6c00, 111);
        put32(arena + 0x6c04, 222);
        put32(arena + 0x100, 1); put32(arena + 0x104, 2);
        put32(arena + 0x110, 0); put32(arena + 0x114, 1);
        for (int i = 0; i < 3; ++i) put32(arena + 0x200 + 28 * i, counts[i]);
        // vertices: 32-byte header per entry, 64-byte vertices, 16 trailing bytes
        unsigned char* s = arena + 0x400;
        float marker = 1.0f;
        std::vector<float*> vtx;
        for (int e = 0; e < 3; ++e) {
            s += 32;
            for (int v = 0; v < counts[e]; ++v) {
                float* f = reinterpret_cast<float*>(s);
                f[0] = marker; f[1] = marker + 0.25f; f[2] = marker + 0.5f;
                float* col = f + 12;
                col[0] = 0.0f; col[1] = 0.5f; col[2] = 1.0f; col[3] = 0.25f * (v & 3);
                marker += 1.0f;
                vtx.push_back(f);
                s += 64;
            }
            s += 16;
        }
        calls.clear();
        cullAt = pickAt = 0;
        cullAnswers = {0, 1, 0};
        pickAnswers = variant == 0 ? std::vector<int>{0} : variant == 1 ? std::vector<int>{1} : std::vector<int>{0, 1};
        int r = PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(nullptr, nullptr, pb, pal);
        // entry 0 (4 vertices) is processed: 2 triangles; entry 1 culled; entry 2 (3 vertices): 1 triangle
        int picks = 0, hits = 0, culls = 0;
        for (const Call& c : calls) {
            picks += c.kind == 1;
            hits += c.kind == 2;
            culls += c.kind == 0;
        }
        if (culls != 3 || picks != 3) return 1;
        if (r != (variant == 0 ? 0 : 1)) return 2;
        if (hits != (variant == 0 ? 0 : variant == 1 ? 3 : 1)) return 3;
        // first pick: material word 222 (group 0 uses material 1), vertices 0, 1, 2 in order
        const Call* first = nullptr;
        for (const Call& c : calls) if (c.kind == 1) { first = &c; break; }
        if (first->material != 222) return 4;
        if (first->v[0] != 1.0f || first->v[3] != 2.0f || first->v[6] != 3.0f) return 5;
        // second pick is flipped: (A, P, B) = vertices 2, 1, 3
        const Call* second = nullptr;
        int seen = 0;
        for (const Call& c : calls) if (c.kind == 1 && ++seen == 2) { second = &c; break; }
        if (second->v[0] != 3.0f || second->v[3] != 2.0f || second->v[6] != 4.0f) return 6;
        // last pick is in the third entry (group 1, material 0 -> word 111) over vertices 6, 7, 8
        const Call* last = nullptr;
        for (const Call& c : calls) if (c.kind == 1) last = &c;
        if (last->material != 111 || last->v[0] != 7.0f || last->v[3] != 8.0f || last->v[6] != 9.0f) return 7;
    }
    return 0;
}
