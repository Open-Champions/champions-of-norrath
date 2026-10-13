#define NORRATH_HOST_CONTRACT 1
unsigned norrath_trail_pool_def __asm__("VITrailFx$sTrailFxPool") = 0;
int norrath_trail_material_def __asm__("VITrailFx$sMaterialHandle") = -1;
#include "trail_fx_clear.cpp"

#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

struct Event { int kind; unsigned a, b, c; };
static std::vector<Event> events;
static unsigned clear_pool_in_dtor = 0xffffffffu;  // if not -1, a dtor call rewrites the pool word

extern "C" void _$_9VITrailFx(void* e, int flags) {
    events.push_back({1, off_of(e), static_cast<unsigned>(flags), 0});
    if (clear_pool_in_dtor != 0xffffffffu) norrath_trail_pool_def = clear_pool_in_dtor;
}
extern "C" void __builtin_vec_delete(void* b) { events.push_back({2, off_of(b), 0, 0}); }
extern "C" int ReleaseMaterial__8VIRasteriR12VIDictionary(VIRaster* r, int h, VIDictionary* d) {
    events.push_back({3, static_cast<unsigned>(reinterpret_cast<unsigned long>(r)), static_cast<unsigned>(h),
                      static_cast<unsigned>(reinterpret_cast<unsigned long>(d))});
    return 7;
}

int main() {
    VIRaster* raster = reinterpret_cast<VIRaster*>(0x500);
    VIDictionary* dict = reinterpret_cast<VIDictionary*>(0x600);
    // pool of 3 elements at guest 0x1008 (header at 0x1000)
    std::memset(arena, 0, sizeof arena);
    unsigned count = 3; std::memcpy(arena + 0x1000, &count, 4);
    norrath_trail_pool_def = 0x1008; norrath_trail_material_def = 12;
    if (VITrailFxClear__FR8VIRasterR12VIDictionary(raster, dict) != 0) return 1;
    if (events.size() != 5) return 2;
    unsigned want[5][2] = {{1, 0x1008 + 56 * 2}, {1, 0x1008 + 56}, {1, 0x1008}, {2, 0x1000}, {3, 0x500}};
    for (int i = 0; i < 5; ++i) if (events[i].kind != (int)want[i][0] || events[i].a != want[i][1]) return 3;
    if (events[0].b != 2 || events[4].b != 12 || events[4].c != 0x600) return 4;
    if (norrath_trail_material_def != -1) return 5;
    if (norrath_trail_pool_def != 0x1008) return 6;     // the pool word is left alone
    // zero elements: no destructor calls, still deleted
    events.clear(); count = 0; std::memcpy(arena + 0x1000, &count, 4); norrath_trail_material_def = -1;
    VITrailFxClear__FR8VIRasterR12VIDictionary(raster, dict);
    if (events.size() != 1 || events[0].kind != 2 || events[0].a != 0x1000) return 7;
    // no pool, material already released
    events.clear(); norrath_trail_pool_def = 0;
    VITrailFxClear__FR8VIRasterR12VIDictionary(raster, dict);
    if (!events.empty() || norrath_trail_material_def != -1) return 8;
    // material set, no pool
    norrath_trail_material_def = 0;
    VITrailFxClear__FR8VIRasterR12VIDictionary(raster, dict);
    if (events.size() != 1 || events[0].kind != 3 || events[0].b != 0 || norrath_trail_material_def != -1) return 9;
    // destructor that rewrites the pool word stops the loop once it equals the element just reached
    events.clear(); count = 3; std::memcpy(arena + 0x1000, &count, 4);
    norrath_trail_pool_def = 0x1008; clear_pool_in_dtor = 0x1008 + 56;
    VITrailFxClear__FR8VIRasterR12VIDictionary(raster, dict);
    if (events.size() != 3 || events[0].kind != 1 || events[1].kind != 1 || events[2].kind != 2 || events[2].a != 0x1008 + 56 - 8) return 10;
    return 0;
}
