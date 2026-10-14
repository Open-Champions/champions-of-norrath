#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../savepoint.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_ctors_lava_wraith_view_42c570[4], norrath_ctors_lava_wraith_view_449550[4], norrath_ctors_lava_wraith_view_4853b8[8];
static unsigned char* obj;
static unsigned char model[32], chunk_source[8];
static unsigned model_low;
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%d args=%d", map == norrath_ctors_lava_wraith_view_42c570, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12); put(obj + 36, model_low, 4); put(obj + 12, 0x20, 4);
    return 1;
}
extern "C" void norrath_light_add(void* l) {
    event("lightAdd %d", int((unsigned char*)l - obj));
    // the record must be complete when lightAdd runs
    expect(obj[144] == 0 && obj[145] == 0, "light bytes");
    expect(word_at(obj + 12) == 0x20, "flags unchanged before lightAdd");
}
extern "C" void norrath_light_deactivate(void* l) { event("lightDeactivate %d flags=%08x b196=%02x", int((unsigned char*)l - obj), word_at(obj + 12), obj[196]); }
extern "C" unsigned norrath_map_add_icon(const void* p, int kind, char* name, int type, char* a, char* b, char* c, char* d) {
    event("icon p=%02x kind=%d name=%d type=%d %d%d%d%d", ((const unsigned char*)p)[0], kind, name == (char*)norrath_ctors_lava_wraith_view_4853b8, type, !!a, !!b, !!c, !!d);
    return 0;
}
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_source, n); return 0xaabbccdd01020304ull; }
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run b196=%02x", obj[196]); return 0x99; }
int main() {
    Image<256> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(chunk_source);
    model_low = low(model);
    put(model + 12, low(chunk_source), 4);
    o.w(12, 0x20);
    const unsigned r = norrath_construct_Savepoint(o.b, pt, (short)0x8001, args);
    expect_log({"GameObject map=1 args=1", "lightAdd 136", "lightDeactivate 136 flags=00100028 b196=5a",
                "icon p=01 kind=0 name=1 type=0 0000", "animInit 76", "chunks 1 0", "run b196=5a"}, "savepoint");
    for (int i = 0; i < 12; ++i) { want.b[16 + i] = pt[i]; want.b[184 + i] = pt[i]; }
    want.w(36, model_low); want.w(12, 0x00100028u); want.w(72, low(norrath_ctors_lava_wraith_view_449550));
    want.w(81, 1, 1); want.w(145, 0, 1); want.w(144, 0, 1);
    volatile float zero = 0.0f; const float q = 1.0f / zero;
    want.w(176, fbits(q)); want.w(180, 0); want.w(164, 0); want.w(172, 0); want.w(168, fbits(q));
    want.w(158, 300, 2); want.w(150, 300, 2); want.w(160, 0xfff0, 2); want.w(152, 0xfff0, 2);
    for (unsigned off : {146u, 148u, 154u, 156u}) want.w(off, 140, 2);
    want.w(40, 36, 1); want.w(132, 0); want.w(197, 0, 1); want.w(198, 0, 1);
    want.w(56, 0x8001, 2); want.w(120, 0x01020304u); want.w(124, 0xaabbccddu); want.w(196, 1, 1);
    expect_image(o, want, "savepoint image");
    expect(r == 0x99u, "savepoint return is the run-list result");
    return finish("savepoint constructor");
}
