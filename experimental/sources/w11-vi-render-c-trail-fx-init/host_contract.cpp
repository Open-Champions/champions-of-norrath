int norrath_trail_material_def __asm__("VITrailFx$sMaterialHandle") = 55;
unsigned norrath_trail_free_def[4] __asm__("VITrailFx$sFreeTrailFx");
unsigned norrath_trail_active_def[4] __asm__("VITrailFx$sActiveTrailFx");
unsigned norrath_trail_inactive_def[4] __asm__("VITrailFx$sInactiveTrailFx");
unsigned norrath_trail_pool_def __asm__("VITrailFx$sTrailFxPool") = 77;
#include "trail_fx_init.cpp"

#include <cstring>
#include <cstdio>
#include <vector>

// Elements are 56 bytes; the host pool pointer is stored in a 32-bit word, so the test uses a
// block inside the low 4 GiB where possible by checking only pointer differences.
struct Ev { int kind; const void* a; long b, c; };
static std::vector<Ev> ev;
static int create_result;
static void* material_ptr = reinterpret_cast<void*>(0x7000);
static void* vec_new_result;
static unsigned long raster_seen;

extern "C" int CreateMaterial__8VIRasteri(VIRaster* r, int a) { ev.push_back({1, r, a, 0}); return create_result; }
extern "C" void* Material__8VIRasteri(VIRaster* r, int h) { ev.push_back({2, r, h, 0}); return material_ptr; }
extern "C" void Init__10VIMateriali(void* m, int a) { ev.push_back({3, m, a, 0}); }
extern "C" void SetLayerFillType__10VIMateriali18VIMaterialFillType(void* m, int l, int t) { ev.push_back({4, m, l, t}); }
extern "C" void SetLayerModulate__10VIMaterialii(void* m, int l, int o) { ev.push_back({5, m, l, o}); }
extern "C" void SetDither__10VIMateriali(void* m, int o) { ev.push_back({6, m, o, 0}); }
extern "C" void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* m, int l, int mode) { ev.push_back({7, m, l, mode}); }
extern "C" void SetZWrite__10VIMateriali(void* m, int o) { ev.push_back({8, m, o, 0}); }
extern "C" void SetZTest__10VIMateriali(void* m, int o) { ev.push_back({9, m, o, 0}); }
extern "C" void Init__7VILList(void* l) { ev.push_back({10, l, 0, 0}); }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { ev.push_back({11, l, (long)n, 0}); }
extern "C" void* __builtin_vec_new(unsigned size) { ev.push_back({12, nullptr, size, 0}); return vec_new_result; }
extern "C" void __9VITrailFx(void* e) { ev.push_back({13, e, 0, 0}); }

int main() {
    VIRaster* raster = reinterpret_cast<VIRaster*>(0x500);
    // failure of CreateMaterial: handle stored, nothing else
    create_result = -1;
    if (VITrailFxInit__FR8VIRasterR12VIDictionary(raster, 0) != -1) return 1;
    if (ev.size() != 1 || norrath_trail_material_def != -1 || norrath_trail_pool_def != 77) return 2;
    // success with an allocation inside the low 4 GiB
    ev.clear(); create_result = 9;
    static char block[904 + 16] __attribute__((aligned(8)));
    vec_new_result = block;
    if (VITrailFxInit__FR8VIRasterR12VIDictionary(raster, 0) != 0) return 3;
    unsigned long base = reinterpret_cast<unsigned long>(block);
    if (norrath_trail_material_def != 9) return 4;
    int k = 0;
    auto at = [&](int i) -> Ev& { return ev[i]; };
    if (at(k).kind != 1 || at(k).b != 0) return 5; ++k;
    if (at(k).kind != 2 || at(k).b != 9) return 6; ++k;
    if (at(k).kind != 3 || at(k).a != material_ptr || at(k).b != 1) return 7; ++k;
    if (at(k).kind != 4 || at(k).b != 0 || at(k).c != 0) return 8; ++k;
    if (at(k).kind != 5 || at(k).b != 0 || at(k).c != 1) return 9; ++k;
    if (at(k).kind != 6 || at(k).b != 0) return 10; ++k;
    if (at(k).kind != 7 || at(k).b != 0 || at(k).c != 1) return 11; ++k;
    if (at(k).kind != 8 || at(k).b != 0) return 12; ++k;
    if (at(k).kind != 9 || at(k).b != 1) return 13; ++k;
    if (at(k).kind != 10 || at(k).a != norrath_trail_free_def) return 14; ++k;
    if (at(k).kind != 10 || at(k).a != norrath_trail_active_def) return 15; ++k;
    if (at(k).kind != 10 || at(k).a != norrath_trail_inactive_def) return 16; ++k;
    if (at(k).kind != 12 || at(k).b != 904) return 17; ++k;
    int header;
    std::memcpy(&header, block, 4);
    if (header != 16) return 18;
    for (int i = 0; i < 16; ++i, ++k)
        if (at(k).kind != 13 || reinterpret_cast<unsigned long>(at(k).a) != base + 8 + 56 * i) return 19;
    unsigned long pool_low = base + 8;
    if (norrath_trail_pool_def != static_cast<unsigned>(pool_low)) return 20;
    for (int i = 0; i < 16; ++i, ++k)
        if (at(k).kind != 11 || at(k).a != norrath_trail_free_def ||
            static_cast<unsigned long>(at(k).b) != (norrath_trail_pool_def + 56ul * i)) return 21;
    if ((size_t)k != ev.size()) return 22;
    // allocation failure: pool word zero, the free list still receives pool + 56 * i
    ev.clear(); vec_new_result = nullptr;
    VITrailFxInit__FR8VIRasterR12VIDictionary(raster, 0);
    if (norrath_trail_pool_def != 0) return 23;
    int adds = 0;
    for (auto& e : ev) if (e.kind == 11) { if (e.b != 56 * adds) return 24; ++adds; }
    if (adds != 16) return 25;
    return 0;
}
