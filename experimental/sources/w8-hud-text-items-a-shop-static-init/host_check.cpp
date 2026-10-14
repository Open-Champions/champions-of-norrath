// Host contract for the shop constructor/destructor entries. Not an EE plan input.
#include <cstring>
#include "shop_static_init.cpp"
static int nctor; static void* ctorObject; static int ints[8], tail[3]; static float fl[3]; static void (*cb)(void*);
static void* dtorObject; static int dtorFlags = -1, ndtor;
extern "C" {
void _$_14AnimationState(void* o, int f) { dtorObject = o; dtorFlags = f; ++ndtor; }
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* o, int a, int b, int c, int d, int e, int f, int g, int h,
                                         float x, float y, float z, int i, int j, int k, void (*callback)(void*)) {
    ++nctor; ctorObject = o; int v[8] = {a, b, c, d, e, f, g, h}; std::memcpy(ints, v, sizeof v);
    fl[0] = x; fl[1] = y; fl[2] = z; tail[0] = i; tail[1] = j; tail[2] = k; cb = callback;
}
}
int main() {
    std::memset(shopList, 0x5a, sizeof shopList); std::memset(norrath_shop_anim_state_view, 0x5a, sizeof norrath_shop_anim_state_view);
    _GLOBAL_$I$shopActiveFrame();
    if (nctor != 1 || ctorObject != candleParticle || ndtor != 0) return 1;
    const int want[8] = {128, 96, 26, 152, 119, 55, 1, 47};
    if (std::memcmp(ints, want, sizeof want) != 0) return 2;
    if (fl[0] != bits(0x40266666) || fl[1] != bits(0x3ef0a3d7) || fl[2] != bits(0x3e0f5c28)) return 3;
    if (tail[0] != 1 || tail[1] != 1 || tail[2] != 4 || cb != nullptr) return 4;
    for (unsigned i = 0; i < sizeof norrath_shop_anim_state_view; ++i)
        if (norrath_shop_anim_state_view[i] != (i == 5 ? 8 : 0x5a)) return 5;
    for (int block = 0; block < 2; ++block) {
        for (int slot = 0; slot < 64; ++slot) {
            const unsigned char* d = shopList + block * 4616 + slot * 72;
            for (int i = 0; i < 72; ++i) {
                unsigned char expect = 0x5a;
                if (i == 32 || i == 33) expect = 0xff;
                else if (i >= 16 && i < 32) expect = 0;
                else if (i == 36 || i == 37 || i == 44 || i == 45 || i == 52 || i == 53 || i == 60 || i == 61) expect = 0xff;
                if (d[i] != expect) return 6;
            }
        }
        for (int i = 4608; i < 4616; ++i) if (shopList[block * 4616 + i] != 0x5a) return 7;
    }
    _GLOBAL_$D$shopActiveFrame();
    if (ndtor != 1 || dtorObject != norrath_shop_anim_state_view || dtorFlags != 0 || nctor != 1) return 8;
    staticInitShop(1, 0xfffe); staticInitShop(0, 0); staticInitShop(2, 0xffff);
    if (ndtor != 1 || nctor != 1) return 9;
    return 0;
}
