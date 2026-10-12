#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../super_orc.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42ce60[4], norrath_ctors_lava_wraith_view_44b260[4];
static unsigned char* obj;
static unsigned char model[160], header[8], chunk_src[8];
static unsigned model_low;
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42ce60, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %g %08x %08x %08x %g %d", a, fbits(b), fbits(c), fbits(d), e, flag); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d b244=%u", m == chunk_src, n, word_at(obj + 244)); return 0x0102030405060708ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) { event("events %d %08x %d %08x %d %08x %d %g %d", a, fbits(b), c, fbits(d), e, fbits(ff), g, h, i); }
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish b100=%08x", word_at(obj + 100)); return 0x77; }
int main() {
    Image<1112> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header); norrath_ctors_lava_wraith::host_note(chunk_src);
    model_low = low(model);
    put(model + 68, low(header), 4); put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_SuperOrc(o.b, pt, (short)-30, args);
    expect_log({"Creature type=-30 map=1 scale=10 args=1", "params 24 3fb33333 3ff99999 40866666 10.5 1", "chunks 1 0 b244=0", "animInit 288", "animAdd 288 1 1", "slow run",
                "events 3 3dcccccc 1 00000000 0 00000000 0 0 0", "events 4 3e999999 1 3f4ccccc 2 3fa66666 4 0 0",
                "events 5 3f866666 1 00000000 0 00000000 0 0 0", "events 6 3e999999 1 3f333333 2 00000000 0 0 0", "finish b100=3f000000"}, "super orc");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_44b260));
    for (unsigned i = 0; i < 4; ++i) want.w(1100 - 8 * i, 0xffff, 2);
    want.w(1072, 0xff, 1); want.w(1073, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1056 + 2 * i, 0, 2); want.w(1064 + 2 * i, 0, 2); }
    want.w(40, 80, 1); want.w(160, 0x21 | 0x20000000u); want.w(164, 0x4000 | 0x20u); want.w(28, fbits(20.0f)); want.w(172, 9, 1);
    want.w(168, 0x3f999999u); want.w(248, 0xcafef00du); want.w(216, 1); want.w(224, 0x3fa66666u);
    for (unsigned off : {176u, 192u, 236u, 964u, 968u, 244u}) want.w(off, 0);
    want.w(572, 0, 2); want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    for (unsigned off : {88u, 92u, 96u, 100u, 104u}) want.w(off, fbits(0.5f));
    expect_image(o, want, "super orc image");
    expect(r == 0x77u, "super orc return");
    return finish("super orc constructor");
}
