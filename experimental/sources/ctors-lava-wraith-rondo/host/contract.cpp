#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../rondo.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42d048[4], norrath_ctors_lava_wraith_view_44f3d8[4];
static unsigned char* obj;
static unsigned stub_model_low;
static unsigned char model[64], header[8], chunk_source[8];
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42d048, scale, arguments != nullptr);
    obj = (unsigned char*)object;
    std::memcpy(obj + 16, point, 12);
    // the base constructor leaves model, flags and a doubleword at +160 for the derived class
    put(obj + 36, stub_model_low, 4);
    put(obj + 12, 0x6, 4);
    put(obj + 160, 0x11, 4); put(obj + 164, 0x2000, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) {
    event("params %g %g %g %g %g %d flags=%08x w76=%08x", a, b, c, d, e, flag, word_at(obj + 12), word_at(obj + 960));
}
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, f); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_source, n); return 0x1122334455667788ull; }
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float f, int g, float h, int i) {
    event("events %d %08x %d %g %d %g %d %g %d", a, fbits(b), c, d, e, f, g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(unsigned flags0, unsigned long long dword0) {
    (void)flags0; (void)dword0;
    calls.clear();
    Image<1000> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(chunk_source); norrath_ctors_lava_wraith::host_note(header);
    put(model + 44, low(header), 4);
    put(model + 12, low(chunk_source), 4);
    put(norrath_actor_view_4844a4, 0xdeadbeefu, 4);
    stub_model_low = low(model);
    const unsigned r = norrath_construct_Rondo(o.b, pt, (short)0xfffe, args);
    expect_log({"Creature type=-2 map=1 scale=10 args=1",
                "params 1.5 1.5 1.5 1.5 1.5 1 flags=01000006 w76=00000000",
                "animInit 288", "animAdd 288 1 1", "chunks 1 1", "run",
                "events 3 3e999999 1 0 0 0 0 0 0", "finish"}, "rondo");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, stub_model_low); want.w(12, 0x01000006u);
    want.w(160, 0x11); want.w(164, 0x2000 | 0x00040004u);
    want.w(72, low(norrath_ctors_lava_wraith_view_44f3d8)); want.w(168, 0x3f999999u); want.w(28, fbits(20.0f));
    want.w(248, 0xdeadbeefu); want.w(792, 0xffff, 2); want.w(572, 0, 2);
    for (unsigned off : {960u, 964u, 176u, 192u, 256u}) want.w(off, 0);
    want.w(236, 1); want.w(244, 1); want.w(552, 0x55667788u); want.w(556, 0x11223344u);
    expect_image(o, want, "rondo image");
    expect(r == 0x77u, "rondo return");
}
int main() {
    run_case(0, 0);
    return finish("rondo constructor");
}
