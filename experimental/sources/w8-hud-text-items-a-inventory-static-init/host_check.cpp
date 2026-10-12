// Host contract for the inventory constructor/destructor entries. Not an EE plan input.
#include <cstring>
#include "inventory_static_init.cpp"
struct Ctor { void* object; int i[8]; float f[3]; int j[3]; void (*cb)(void*); };
static Ctor ctors[4]; static int nctor;
static void* dtors[8]; static int dflags[8]; static int ndtor;
extern "C" {
void _$_14AnimationState(void* object, int flags) { dtors[ndtor] = object; dflags[ndtor] = flags; ++ndtor; }
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* o, int a, int b, int c, int d, int e, int f, int g, int h,
                                         float x, float y, float z, int i, int j, int k, void (*cb)(void*)) {
    Ctor& c0 = ctors[nctor++]; c0.object = o; int v[8] = {a, b, c, d, e, f, g, h}; std::memcpy(c0.i, v, sizeof v);
    c0.f[0] = x; c0.f[1] = y; c0.f[2] = z; c0.j[0] = i; c0.j[1] = j; c0.j[2] = k; c0.cb = cb;
}
}
int main() {
    unsigned char* a = norrath_inventory_anim_state_view;
    std::memset(a, 0x5a, sizeof norrath_inventory_anim_state_view);
    _GLOBAL_$I$inventoryLoad__Fv();
    for (unsigned i = 0; i < sizeof norrath_inventory_anim_state_view; ++i)
        if (a[i] != ((i % 264 == 5) ? 8 : 0x5a)) return 1;
    if (ndtor != 0 || nctor != 2) return 2;
    const int t[8] = {238, 210, 100, 180, 105, 63, 4, 0};
    const int h[8] = {5, 128, 128, 402, 128, 128, 128, -68};
    if (ctors[0].object != torchParticle || std::memcmp(ctors[0].i, t, sizeof t) != 0) return 3;
    if (ctors[0].f[0] != 30.0f || ctors[0].f[1] != 5.0f || ctors[0].f[2] != bits(0x3f733333)) return 4;
    if (ctors[0].j[0] != 2 || ctors[0].j[1] != 1 || ctors[0].j[2] != 0 || ctors[0].cb != nullptr) return 5;
    if (ctors[1].object != heatParticle || std::memcmp(ctors[1].i, h, sizeof h) != 0) return 6;
    if (ctors[1].f[0] != 38.0f || ctors[1].f[1] != 34.0f || ctors[1].f[2] != 0.0f) return 7;
    if (ctors[1].j[0] != 2 || ctors[1].j[1] != 3 || ctors[1].j[2] != 0 || ctors[1].cb != nullptr) return 8;
    _GLOBAL_$D$inventoryLoad__Fv();
    if (ndtor != 4 || nctor != 2) return 9;
    for (int i = 0; i < 4; ++i) if (dtors[i] != a + (3 - i) * 264 || dflags[i] != 0) return 10;
    ndtor = 0;
    staticInitInventory(1, 0xfffe); staticInitInventory(0, 0); staticInitInventory(2, 0xffff);
    if (ndtor != 0 || nctor != 2) return 11;
    return 0;
}
