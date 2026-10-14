// Host contract for the LightEffect constructor. Not an EE plan input.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
unsigned frameNm = 1000;
unsigned char vt_11LightEffect[16] __asm__("_vt$11LightEffect");
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void*, const void*, float, float, float, float, float, float,
                                                        float, float, float, float, float, float, void*, int);
}
static std::vector<const char*> order;
static unsigned char* current = nullptr;
static float point_seen[3];
static unsigned char light_at_add[64];
static unsigned char object_at_run[200];
static void* link_args[3];
static unsigned word(const unsigned char* b, unsigned o) { unsigned v; std::memcpy(&v, b + o, 4); return v; }
static float flt(const unsigned char* b, unsigned o) { float v; std::memcpy(&v, b + o, 4); return v; }
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    order.push_back("construct");
    std::memcpy(point_seen, point, 12);
    if (map || arguments) std::fprintf(stderr, "fail: construct nulls\n");
    unsigned flags = 0x00200000u;
    std::memcpy(static_cast<unsigned char*>(object) + 12, &flags, 4);
    return 0;
}
extern "C" void norrath_light_add(void* light) {
    order.push_back("lightAdd");
    std::memcpy(light_at_add, light, sizeof light_at_add);
    if (light != current + 76) std::fprintf(stderr, "fail: light address\n");
}
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value) {
    order.push_back("link");
    link_args[0] = head; link_args[1] = object_head; link_args[2] = value;
    return nullptr;
}
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    order.push_back("run");
    std::memcpy(object_at_run, object, sizeof object_at_run);
    return 0;
}

int main() {
    alignas(16) unsigned char object[200];
    std::memset(object, 0xcc, sizeof object);
    current = object;
    int owner_storage = 0;
    const float point[3] = {5.0f, 6.0f, 7.0f};
    __11LightEffectG6Point3ffffffffffffP10GameObjecti(object, point, 10.5f, 20.5f, -30.5f, 40.5f, 50.0f, 60.0f, 70.0f,
                                                       80.0f, 4.0f, 14.0f, 9.0f, 0.5f, &owner_storage, 0x77);
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    const char* want[] = {"construct", "lightAdd", "link", "run"};
    expect(order.size() == 4, "four calls with an owner");
    for (size_t i = 0; i < order.size() && i < 4; ++i) expect(std::strcmp(order[i], want[i]) == 0, "call order");
    expect(point_seen[0] == 5.0f && point_seen[2] == 7.0f, "point passed");
    expect(word(object, 72) == static_cast<unsigned>(reinterpret_cast<unsigned long>(vt_11LightEffect)), "vtable");
    expect(std::memcmp(object + 124, point, 12) == 0, "point copied to +124");
    short h[8];
    const unsigned offs[8] = {10, 12, 14, 16, 18, 20, 22, 24};
    for (int i = 0; i < 8; ++i) std::memcpy(&h[i], object + 76 + offs[i], 2);
    const short want_h[4] = {10, 20, -30, 40};   // truncated toward zero
    for (int i = 0; i < 8; ++i) expect(h[i] == want_h[i % 4], "light halfwords truncated");
    expect(flt(object, 76 + 28) == 4.0f && flt(object, 76 + 36) == 4.0f, "radius words");
    expect(std::fabs(flt(object, 76 + 40) - 1.0f / 16.0f) < 1e-7f && flt(object, 76 + 32) == flt(object, 76 + 40), "inverse square words");
    expect(flt(object, 76 + 44) == 9.0f, "f11 word");
    expect(light_at_add[8] == 0 && light_at_add[9] == 0 && object[85] == 0, "light flag bytes");
    expect(word(object, 136) == 0u, "+136 cleared");
    expect(word(object, 12) == (0x00200000u | 3u), "flags ORed with 3");
    expect(link_args[0] == object + 136 && link_args[1] == &owner_storage && link_args[2] == &owner_storage, "owner link");
    // 0.5 s -> 30 frames.
    expect(word(object, 180) == 1030u, "end frame");
    expect(flt(object, 140) == 10.5f && flt(object, 152) == 40.5f && flt(object, 172) == 4.0f, "start floats");
    expect(std::fabs(flt(object, 156) - (50.0f - 10.5f) / 30.0f) < 1e-5f, "step 1");
    expect(std::fabs(flt(object, 160) - (60.0f - 20.5f) / 30.0f) < 1e-5f, "step 2");
    expect(std::fabs(flt(object, 164) - (70.0f + 30.5f) / 30.0f) < 1e-5f, "step 3");
    expect(std::fabs(flt(object, 168) - (80.0f - 40.5f) / 30.0f) < 1e-5f, "step 4");
    expect(std::fabs(flt(object, 176) - (14.0f - 4.0f) / 30.0f) < 1e-5f, "radius step");
    expect(word(object_at_run, 168) == word(object, 168), "+168 stored before the run-list call");
    expect(word(object_at_run, 184) == 0xccccccccu && word(object, 184) == 0x77u, "+184 stored after the run-list call");

    // No owner: no link call. Zero seconds: one frame.
    order.clear();
    std::memset(object, 0xcc, sizeof object);
    __11LightEffectG6Point3ffffffffffffP10GameObjecti(object, point, 1, 2, 3, 4, 5, 6, 7, 8, 2.0f, 3.0f, 0.0f, 0.0f, nullptr, 1);
    expect(order.size() == 3, "no link without an owner");
    expect(word(object, 180) == 1001u, "one frame minimum");
    expect(std::fabs(flt(object, 156) - 4.0f) < 1e-5f, "full delta in one frame");
    if (bad) return 1;
    std::printf("light-effect-ctor contract passed\n");
    return 0;
}
