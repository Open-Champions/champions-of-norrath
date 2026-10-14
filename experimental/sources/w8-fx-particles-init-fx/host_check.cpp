// Host contract for _GLOBAL_$I$fxResourceMap. Not an EE plan input.
// The expected tuples below come from running the executable's own initializer on a small interpreter, not
// from the authored source; the object identities are checked by docs/agents/w8-fx-particles-compare.py.
#include <cstdio>
#include <cstring>

extern "C" {
void _GLOBAL_$I$fxResourceMap();
unsigned char fxDust[80];
}

struct Call { int ints[8]; unsigned floats[3]; int mode, tier, grid; };
static const Call expected[] = {
    {{92, 90, 64, 34, 92, 85, 64, 0}, {0x41a570a3u, 0x4118ccccu, 0xc0a0f5c2u}, 2, 0, 2},
    {{48, 6, 9, 204, 30, 6, 6, 0}, {0x41280000u, 0x3f999999u, 0xc094ccccu}, 3, 0, 6},
    {{80, 70, 5, 204, 60, 40, 5, 0}, {0x41280000u, 0x3f999999u, 0xc094ccccu}, 3, 0, 6},
    {{39, 108, 40, 53, 40, 87, 19, 0}, {0x41280000u, 0x3f999999u, 0xc094ccccu}, 3, 0, 6},
    {{100, 210, 238, 90, 4, 63, 105, 0}, {0x40b3d70au, 0x40b80000u, 0xc0000000u}, 1, 1, 4},
    {{48, 6, 9, 204, 30, 6, 6, 0}, {0x40900000u, 0x3f999999u, 0xc0299999u}, 1, 0, 6},
    {{48, 6, 9, 204, 30, 6, 6, 0}, {0x40900000u, 0x3f999999u, 0xc0299999u}, 3, 0, 6},
    {{213, 211, 49, 90, 438, 63, 4, 0}, {0x4139fbe7u, 0x42bad70au, 0x00000000u}, 1, 1, 4},
    {{225, 169, 125, 118, 163, 131, 102, 19}, {0x407cfdf3u, 0x4298ab02u, 0x00000000u}, 1, 2, 5},
    {{100, 210, 238, 90, 4, 63, 105, 0}, {0x4139eb85u, 0x424f0000u, 0x00000000u}, 1, 1, 4},
    {{100, 210, 238, 90, 4, 63, 105, 0}, {0x40b3d70au, 0x40b80000u, 0xc0000000u}, 3, 1, 4},
    {{238, 210, 100, 180, 105, 63, 4, 0}, {0x41f00000u, 0x40a00000u, 0x3f733333u}, 2, 1, 0},
    {{261, 290, 64, 257, 267, 20, 25, 0}, {0x42840000u, 0x430c0000u, 0x00000000u}, 1, 1, 2},
    {{248, 283, -21, 128, 251, 144, 11, 32}, {0x42680000u, 0x42140000u, 0x00000000u}, 1, 1, 6},
    {{261, 290, 64, 257, 267, 20, 25, 0}, {0x41199999u, 0x41099999u, 0x00000000u}, 2, 1, 8},
    {{64, 261, 290, 257, 25, 20, 267, 0}, {0x41199999u, 0x41099999u, 0x00000000u}, 2, 1, 8},
    {{20, 20, 20, 100, 15, 15, 15, 0}, {0x41000000u, 0x40b33333u, 0x00000000u}, 2, 0, 9},
    {{201, 191, 103, 70, 129, 20, 74, 0}, {0x41c00000u, 0x41200000u, 0x40ae6666u}, 1, 1, 8},
    {{64, 39, 15, 82, 0, 0, 0, 0}, {0x41700000u, 0x427c0000u, 0x3fc00000u}, 3, 0, 6},
    {{100, 72, 40, 41, 145, 89, 25, 0}, {0x41c40000u, 0x410c0000u, 0x00000000u}, 1, 1, 4},
    {{2, 83, 74, 224, 31, 43, 31, 214}, {0x41b95c28u, 0x41bdb645u, 0x00000000u}, 3, 3, 2},
    {{220, 62, 30, 41, 250, 79, 15, 0}, {0x41f40000u, 0x41c60000u, 0x00000000u}, 1, 1, 4},
    {{333, 358, 287, 128, 122, 106, 349, 0}, {0x40be147au, 0x41091eb8u, 0xc1151eb8u}, 1, 2, 0},
    {{111, 112, 154, 80, 33, 30, 28, 0}, {0x41000000u, 0x42960000u, 0x3dccccccu}, 2, 0, 6},
    {{111, 112, 154, 60, 33, 30, 28, -30}, {0x41700000u, 0x41f00000u, 0x3dccccccu}, 0, 0, 6},
    {{231, 87, 100, 29, 264, 63, 4, 0}, {0x41a8b851u, 0x419accccu, 0x3f9851ebu}, 0, 1, 0},
    {{295, 195, 111, 413, 226, 11, 14, -11}, {0x40800000u, 0x40400000u, 0xc0c00000u}, 1, 1, 0},
    {{130, 116, 76, 87, 194, 120, 74, 0}, {0x41a00000u, 0x00000000u, 0x3fb33333u}, 0, 1, 6},
    {{0, 0, 0, 64, 34, 30, 21, -32}, {0x41700000u, 0x41f00000u, 0x40400000u}, 1, 0, 6},
    {{100, 210, 238, 80, 4, 63, 105, -50}, {0x41400000u, 0xc1400000u, 0xc0e00000u}, 0, 1, 8},
    {{100, 210, 235, 140, 198, 199, 244, 140}, {0x40800000u, 0x00000000u, 0xc0000000u}, 2, 1, 5},
    {{100, 139, 255, 255, 198, 199, 244, 255}, {0x40e00000u, 0x00000000u, 0xc104ccccu}, 2, 0, 5},
    {{48, 6, 9, 204, 30, 6, 6, 0}, {0x41280000u, 0x3f999999u, 0xc094ccccu}, 1, 0, 6},
    {{80, 70, 5, 204, 60, 40, 5, 0}, {0x41280000u, 0x3f999999u, 0xc094ccccu}, 1, 0, 6},
    {{39, 108, 40, 53, 40, 87, 19, 0}, {0x41280000u, 0x3f999999u, 0xc094ccccu}, 1, 0, 6},
    {{68, 136, 144, 40, 137, 160, 158, 0}, {0x41a00000u, 0x437a0000u, 0x40000000u}, 2, 0, 6},
    {{111, 151, 154, 40, 60, 120, 127, 40}, {0x41200000u, 0x41200000u, 0xc0a00000u}, 2, 0, 6},
    {{68, 136, 144, 40, 137, 160, 158, 0}, {0x43160000u, 0x43960000u, 0x40900000u}, 2, 0, 6},
    {{121, 127, 126, 145, 66, 87, 68, 0}, {0x40b00000u, 0x40a851ebu, 0xc1cc51ebu}, 2, 0, 2},
    {{295, 195, 111, 149, 226, 11, 14, 24}, {0x40800000u, 0x40c00000u, 0xc0533333u}, 2, 1, 0},
    {{130, 116, 76, 87, 194, 120, 74, 0}, {0x40800000u, 0x42300000u, 0x402cccccu}, 0, 1, 2},
    {{0, 0, 0, 64, 34, 30, 22, 0}, {0x41600000u, 0x429a0000u, 0x3f800000u}, 3, 0, 6},
    {{0, 0, 0, 64, 34, 30, 22, 0}, {0x41f00000u, 0x42480000u, 0x3f800000u}, 2, 0, 6},
    {{36, 89, 49, 56, 65, 144, 85, 0}, {0x41cd9999u, 0x42a00000u, 0x00000000u}, 2, 1, 6},
    {{92, 89, 64, 20, 92, 85, 64, 0}, {0x4272b851u, 0x42f11999u, 0xc041eb85u}, 2, 0, 2},
    {{100, 210, 235, 140, 198, 199, 244, 140}, {0x40800000u, 0x00000000u, 0xc0e00000u}, 3, 1, 5},
    {{100, 139, 255, 255, 198, 199, 244, 0}, {0x41000000u, 0x40c00000u, 0xc104ccccu}, 3, 0, 5},
    {{144, 148, 173, 298, 120, 117, 114, 4}, {0x4104ccccu, 0x41033333u, 0xbfa66666u}, 2, 1, 8},
    {{117, 137, 215, 121, 148, 147, 223, 5}, {0x40c00000u, 0x41a00000u, 0x00000000u}, 3, 1, 6},
    {{15, 10, 10, 14, 0, 0, 0, 0}, {0x428cccccu, 0x42493333u, 0x00000000u}, 1, 0, 6},
    {{237, 50, 10, 40, 217, 50, 10, 125}, {0x41d40000u, 0x40a00000u, 0xc1a00000u}, 1, 2, 5},
    {{117, 137, 215, 121, 148, 147, 223, 0}, {0x40571a9fu, 0x421b9999u, 0xbeb43958u}, 3, 2, 7},
};
static const int count = 52;
static int calls = 0;
static void* objects[52];
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}
static unsigned float_bits(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }

extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y,
                                         float z, int i, int j, int k, void (*callback)(void*)) {
    if (calls >= count) { ++calls; return; }
    const Call& want = expected[calls];
    const int got[8] = {a, b, c, d, e, f, g, h};
    for (int n = 0; n < 8; ++n) expect(got[n] == want.ints[n], "integer argument");
    expect(float_bits(x) == want.floats[0] && float_bits(y) == want.floats[1] && float_bits(z) == want.floats[2], "float argument");
    expect(i == want.mode && j == want.tier && k == want.grid && callback == nullptr, "trailing arguments");
    objects[calls] = self;
    ++calls;
}

int main() {
    _GLOBAL_$I$fxResourceMap();
    expect(calls == count, "constructor call count");
    for (int n = 0; n < calls && n < count; ++n) {
        for (int m = 0; m < n; ++m) {
            expect(objects[m] != objects[n], "each call has its own object");
        }
    }
    if (failures) return 1;
    std::printf("fxResourceMap contract passed\n");
    return 0;
}
