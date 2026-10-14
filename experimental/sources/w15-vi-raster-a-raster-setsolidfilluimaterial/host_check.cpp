// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int add_calls, flush_calls, add_result, seen_material;
static unsigned char* seen_self; static unsigned char* seen_wp;
extern "C" int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* s, int m, unsigned char* wp) { ++add_calls; seen_self = s; seen_material = m; seen_wp = wp; return add_result; }
extern "C" void FlushDMABufferIfFull__8VIRaster(unsigned char* s) { ++flush_calls; if (s != seen_self) flush_calls += 100; }
#include "set_solid_fill_ui_material.cpp"
#include <cstdio>
#include <initializer_list>
int main() {
    *reinterpret_cast<int*>(arena + 17832) = 42;
    bool ok = true;
    for (int r : {0, 5, -1, -100}) {
        add_calls = flush_calls = 0; add_result = r;
        int v = SetSolidFillUIMaterial__8VIRaster(arena);
        ok = ok && v == (r < 0 ? -1 : 0) && add_calls == 1 && flush_calls == (r < 0 ? 0 : 1) && seen_material == 42 && seen_wp == arena + 19492 && seen_self == arena;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
