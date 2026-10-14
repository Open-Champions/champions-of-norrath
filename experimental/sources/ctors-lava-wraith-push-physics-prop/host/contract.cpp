#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../push_physics_prop.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actors_b_view_448a70[4];
static unsigned char* obj;
static char names[3][4];
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%d args=%d", map != nullptr, arguments != nullptr);
    obj = (unsigned char*)object;
    std::memcpy(obj + 16, point, 12);
    return 1;
}
extern "C" void* norrath_lump_find_resource(char* a, char* b) {
    event("lump %d %d", int(a - names[0]), int(b - names[0]));
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x1000 + (b - names[0])));
}
extern "C" void norrath_body_init(void* body, float a, float b, float c, float d, float e) {
    event("body %d %g %g %g %g %g", int((unsigned char*)body - obj), a, b, c, d, e);
}
extern "C" unsigned norrath_actor_add_run_list(void*) {
    event("run list y=%08x copy=%08x", word_at(obj + 24), word_at(obj + 104));
    return 7;
}
static void run_case(float scale) {
    calls.clear();
    Image<288> o, want;
    unsigned char pt[12];
    put(pt, fbits(10.5f), 4); put(pt + 4, fbits(-3.0f), 4); put(pt + 8, fbits(7.25f), 4);
    o.w(12, 0x80); want.w(12, 0x90);
    norrath_construct_PushPhysicsProp(o.b, pt, 5, names[0], names[1], names[2], scale);
    char b[160];
    std::vector<std::string> w{"GameObject map=0 args=0", "lump 0 4", "lump 0 8"};
    std::snprintf(b, sizeof b, "body 80 1 1 %g %g %g", scale + scale, scale + scale, scale + scale); w.push_back(b);
    std::snprintf(b, sizeof b, "run list y=%08x copy=%08x", fbits(7.25f), fbits(7.25f + scale)); w.push_back(b);
    expect_log(w, "push physics prop");
    for (int i = 0; i < 12; ++i) { want.b[16 + i] = pt[i]; want.b[232 + i] = pt[i]; want.b[96 + i] = pt[i]; }
    // x at +16, y at +20, z at +24; the +96 copy is taken while z is raised by the scale
    want.w(104, fbits(7.25f + scale));
    want.w(256, fbits(scale)); want.w(72, low(norrath_actors_b_view_448a70));
    want.w(276, low(names[2])); want.w(268, low(names[0])); want.w(272, low(names[1]));
    want.w(260, 0x1000 + 4); want.w(264, 0x1000 + 8);
    want.w(76, 0); want.w(32, fbits(scale + scale)); want.w(28, fbits(scale * [] { unsigned u = 0x3f999999u; float f; std::memcpy(&f, &u, 4); return f; }()));
    want.w(244, 0); want.w(248, 0); want.w(252, 0);
    expect_image(o, want, "push physics prop image");
}
int main() {
    run_case(24.0f); run_case(15.0f);
    return finish("push physics prop constructor");
}
