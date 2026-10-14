// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static unsigned char scratch[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    if (address >= 0x70000000u) return scratch + (address - 0x70000000u);
    return arena + (address - arena_base);
}
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int log_ctor, log_invert, init_calls, init_result[2], cull_result, picker_calls, picker_kind, picker_result, buffer_mode;
static unsigned char* init_dest[2]; static unsigned char* init_src[2]; static unsigned char* init_matrix[2];
static unsigned char* seen_buffer; static unsigned char* seen_palette; static unsigned char* seen_bbox;
static unsigned char prim_buffer[64];
extern "C" {
void __13VIPickSegment(unsigned char*) { ++log_ctor; }
void Invert__10VIMatrix44RC10VIMatrix44(unsigned char* d, unsigned char* s) { ++log_invert; if (d != scratch + 0x100 || s != scratch) log_invert += 100; }
int Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44(unsigned char* d, unsigned char* s, unsigned char* m) {
    init_dest[init_calls] = d; init_src[init_calls] = s; init_matrix[init_calls] = m; return init_result[init_calls++];
}
unsigned char* PrimBuffer__8VIRasteri(unsigned char*, int) { *reinterpret_cast<int*>(prim_buffer + 8) = buffer_mode; return prim_buffer; }
int Cull__8VIRasterR13VIPickSegmentR6VIBBox(unsigned char*, unsigned char*, unsigned char* bbox) { seen_bbox = bbox; return cull_result; }
static int picker(int kind, unsigned char*, unsigned char*, unsigned char* b, unsigned char* p) { ++picker_calls; picker_kind = kind; seen_buffer = b; seen_palette = p; return picker_result; }
int PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char* r, unsigned char* s, unsigned char* b, unsigned char* p) { return picker(0, r, s, b, p); }
int PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char* r, unsigned char* s, unsigned char* b, unsigned char* p) { return picker(2, r, s, b, p); }
int PickPrimBufferPackVUNCG__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char* r, unsigned char* s, unsigned char* b, unsigned char* p) { return picker(4, r, s, b, p); }
int PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char* r, unsigned char* s, unsigned char* b, unsigned char* p) { return picker(5, r, s, b, p); }
}
#include "pick_prim_buffer.cpp"
int main() {
    bool ok = true;
    unsigned char segment[96];
    unsigned char* self = arena;
    *reinterpret_cast<unsigned*>(self + 19436) = arena_base + 0x4000;
    *reinterpret_cast<unsigned*>(arena + 0x4000 + 12 * 3) = arena_base + 0x5000;
    float* off = reinterpret_cast<float*>(scratch + 0x200);
    off[0] = 1.0f; off[1] = 2.0f; off[2] = 4.0f; off[3] = 0.5f;
    for (int mode : {0, 1, 2, 3, 4, 5, 6}) for (int cull : {0, 1}) for (int res : {0, 7}) for (int init1 : {0, -1}) {
        std::memset(segment, 0, sizeof segment);
        float* f = reinterpret_cast<float*>(segment + 76);
        f[0] = 10.0f; f[1] = 20.0f; f[2] = 30.0f; f[3] = 40.0f;
        log_ctor = log_invert = init_calls = picker_calls = 0; init_result[0] = 0; init_result[1] = init1;
        cull_result = cull; picker_result = res; buffer_mode = mode;
        int v = PickPrimBuffer__8VIRasterR13VIPickSegmentii(self, segment, 5, 3);
        const bool picked = !cull && (mode == 0 || mode == 2 || mode == 4 || mode == 5);
        const bool hit = picked && res != 0;
        ok = ok && log_ctor == 1 && log_invert == 1 && init_calls == (hit ? 2 : 1);
        ok = ok && init_src[0] == segment && init_matrix[0] == scratch + 0x100;
        ok = ok && seen_bbox == prim_buffer + 20;
        if (picked) ok = ok && picker_calls == 1 && picker_kind == mode && seen_buffer == prim_buffer && seen_palette == arena + 0x5000;
        else ok = ok && picker_calls == 0;
        if (hit && init1 < 0) { ok = ok && v == -1; continue; }
        ok = ok && v == (hit ? 1 : 0);
        if (hit) ok = ok && init_dest[1] == segment && init_src[1] == init_dest[0] && init_matrix[1] == scratch && f[0] == 11.0f && f[1] == 22.0f && f[2] == 34.0f && f[3] == 20.0f;
        else ok = ok && f[0] == 10.0f && f[3] == 40.0f;
    }
    init_calls = 0; init_result[0] = -1;
    ok = ok && PickPrimBuffer__8VIRasterR13VIPickSegmentii(self, segment, 5, 3) == -1 && init_calls == 1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
