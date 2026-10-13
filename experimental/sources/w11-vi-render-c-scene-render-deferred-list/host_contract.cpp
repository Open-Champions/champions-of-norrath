#define NORRATH_HOST_CONTRACT 1
#include "render_deferred_list.cpp"

#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }

struct Ev { std::string what; unsigned a, b, c, d; };
static std::vector<Ev> ev;
static float last_model_view[16];
static float last_colour[4];
static unsigned scene_1092_at_lighting, scene_1096_at_lighting;
static const unsigned scene = 0x1000;

extern "C" int SetPretranslations__6VIZoneR8VIRasterR8VICamera(void* z, unsigned r, void* c) { ev.push_back({"pre", off_of(z), r, off_of(c), 0}); return 0; }
extern "C" void sceVu0CopyMatrix(void* d, const void* s) {
    ev.push_back({"copy", off_of(s), 0, 0, 0});
    std::memcpy(d, s, 64);
}
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(unsigned r, const void* m) {
    std::memcpy(last_model_view, m, 64);
    ev.push_back({"view", r, 0, 0, 0});
}
extern "C" void SetStaticLighting__8VIRasterRC10VIColor32F(unsigned r, const void* c) {
    std::memcpy(last_colour, c, 16);
    scene_1092_at_lighting = get(scene + 1092); scene_1096_at_lighting = get(scene + 1096);
    ev.push_back({"light", r, off_of(c) , 0, 0});
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* r, const void* a, const void* b) {
    ev.push_back({"mul", off_of(a), off_of(b), 0, 0});
    float* out = static_cast<float*>(r);
    for (int i = 0; i < 16; ++i) out[i] = 7.0f + i;
}
extern "C" void _$_10VIMatrix44(void* m, int flags) { ev.push_back({"dtor", (unsigned)flags, 0, 0, 0}); if ((reinterpret_cast<unsigned long>(m) & 15) != 0) ev.push_back({"misaligned", 0, 0, 0, 0}); }
extern "C" void norrath_host_call_draw(unsigned function, char* sprite, void* camera, void* sc, unsigned raster) {
    ev.push_back({"draw", function, off_of(sprite), off_of(camera), raster});
    ev.push_back({"draw-scene", off_of(sc), get(scene + 1092), get(scene + 1096), 0});
}

int main() {
    const unsigned list = scene + 1112, entries = 0x2000, zones = 0x3000, sprites = 0x4000, camera = 0x500, block = 0x6000;
    const unsigned vtable = 0x7000, sprite_obj = 0x7100;
    auto reset = [&]() {
        std::memset(arena, 0, sizeof arena); ev.clear();
        put(scene + 80, zones); put(scene + 19356, 0xabc0); put(scene + 996 + 8, sprites);
        put(list + 8, entries);
        put(sprites + 12 * 3, sprite_obj); put(sprite_obj + 60, vtable - 24);
        put(vtable, 0xfffe0000u | 0x0010); put(vtable + 4, 0x00c0ffee);   // adjust 0x10, function 0xc0ffee
        for (int i = 0; i < 16; ++i) { float f = 100.0f + i; std::memcpy(arena + camera + 48 + 4 * i, &f, 4); }
        put(block + 108, 0x1234); for (int i = 0; i < 4; ++i) put(block + 112 + 4 * i, 0x3f800000u);
        put(scene + 1092, 0x55); put(scene + 1096, 0x66);
    };
    auto element = [&](int i, int zone, int blk, int sprite) { put(entries + 12 * i, zone); put(entries + 12 * i + 4, blk); put(entries + 12 * i + 8, sprite); };
    VIScene* s = reinterpret_cast<VIScene*>(arena + scene);
    VICamera* c = reinterpret_cast<VICamera*>(arena + camera);
    // empty list: only the destructor runs
    reset(); put(list, 0);
    if (RenderDeferredList__7VISceneR8VICameraii(s, c, 1, 1) != 0 || ev.size() != 1 || ev[0].what != "dtor" || ev[0].a != 2) return 1;
    // camera-only element (block word -1), flagB set
    reset(); put(list, 1); element(0, 5, -1, 3);
    RenderDeferredList__7VISceneR8VICameraii(s, c, 0, 1);
    if (ev.size() != 7) return 2;
    if (ev[0].what != "pre" || ev[0].a != zones + 592 * 5 || ev[0].b != 0xabc0 || ev[0].c != camera) return 3;
    if (ev[1].what != "copy" || ev[1].a != camera + 48) return 4;
    if (last_model_view[12] != 0.0f || last_model_view[13] != 0.0f || last_model_view[14] != 0.0f || last_model_view[15] != 1.0f) return 6;
    if (last_model_view[0] != 100.0f || last_model_view[11] != 111.0f) return 7;
    if (ev[2].what != "view" || ev[2].a != 0xabc0) return 8;
    if (ev[3].what != "light" || ev[3].a != 0xabc0) return 9;
    if (last_colour[0] != 0.0f || last_colour[1] != 0.0f || last_colour[2] != 0.0f || last_colour[3] != 1.0f) return 10;
    if (ev[4].what != "draw" || ev[4].a != 0x00c0ffee || ev[4].b != sprite_obj + 0x10 || ev[4].c != camera || ev[4].d != 0xabc0) return 11;
    if (ev[5].what != "draw-scene" || ev[5].a != scene || ev[5].b != 0x55 || ev[5].c != 0x66) return 12;   // scene words untouched in this path
    if (ev[6].what != "dtor") return 13;
    // the same element with flagB clear draws nothing but still pretranslates
    reset(); put(list, 1); element(0, 5, -1, 3);
    RenderDeferredList__7VISceneR8VICameraii(s, c, 1, 0);
    if (ev.size() != 2 || ev[0].what != "pre" || ev[1].what != "dtor") return 14;
    // block element, flagA set: Mul, lighting words around the draw
    reset(); put(list, 1); element(0, -1, block, 3);
    RenderDeferredList__7VISceneR8VICameraii(s, c, 1, 0);
    if (ev.size() != 6) return 15;
    if (ev[0].what != "mul" || ev[0].a != block || ev[0].b != camera + 48) return 16;
    if (last_model_view[0] != 7.0f || last_model_view[15] != 22.0f) return 17;
    if (ev[1].what != "view" || ev[2].what != "light" || ev[2].b != block + 112) return 18;
    if (scene_1092_at_lighting != 0x1234 || scene_1096_at_lighting != 0xffffffffu) return 19;
    if (ev[3].what != "draw" || ev[4].b != 0x1234 || ev[4].c != 0xffffffffu) return 20;
    if (get(scene + 1092) != 0xffffffffu || get(scene + 1096) != 0xffffffffu) return 21;
    // block element with flagA clear: nothing but the destructor
    reset(); put(list, 1); element(0, -1, block, 3);
    RenderDeferredList__7VISceneR8VICameraii(s, c, 0, 1);
    if (ev.size() != 1) return 22;
    // pretranslation is skipped for -1 and for a repeated zone, and `last` follows -1
    reset(); put(list, 5); element(0, 2, -1, 3); element(1, 2, -1, 3); element(2, -1, -1, 3); element(3, 2, -1, 3); element(4, 4, -1, 3);
    RenderDeferredList__7VISceneR8VICameraii(s, c, 0, 0);
    int pre = 0; unsigned zs[8];
    for (auto& e : ev) if (e.what == "pre") zs[pre++] = e.a;
    if (pre != 3 || zs[0] != zones + 592 * 2 || zs[1] != zones + 592 * 2 || zs[2] != zones + 592 * 4) return 23;
    return 0;
}
