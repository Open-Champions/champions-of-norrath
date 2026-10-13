// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "set_state.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
static std::string log_text;
static int sp_result = 0, sp_new = -1000, sp_seen = -1000, roll = 0, dest_range = 0;
static void* sp_object = nullptr;
static void* trans_header[3];
static unsigned trans_rate[3];
static int trans_flags[3];
static int trans_n = 0;
static void* shot_header = nullptr;
static unsigned shot_a = 0, shot_b = 0;
static int shot_flags = 0, drop_kind = 0;
static unsigned flags_at_drop = 0;
static unsigned long long d160_at_drop = 0;
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" int setStateProcessing__8CreaturePi(void* self, int* state) {
    log_text += "p"; sp_object = self; sp_seen = *state;
    if (sp_new != -1000) *state = sp_new;
    return sp_result;
}
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float rate, int flags) {
    log_text += "t"; if (trans_n < 3) { trans_header[trans_n] = h; trans_rate[trans_n] = bits_of(rate); trans_flags[trans_n] = flags; ++trans_n; }
    (void)s;
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int flags) {
    log_text += "o"; shot_header = h; shot_a = bits_of(a); shot_b = bits_of(b); shot_flags = flags;
}
extern "C" void animGlobalSync__FP14AnimationState(void*) { log_text += "g"; }
extern "C" void resetRouting__8Creature(void*) { log_text += "z"; }
extern "C" void getSmartRandomDestination__8Creaturei(void* r, void*, int range) {
    log_text += "d"; dest_range = range;
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(r)[i] = static_cast<unsigned char>(0x41 + i);
}
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return roll; }
static unsigned char object[1300];
extern "C" void dropItem__8Skelighti(void*, int kind) {
    log_text += "x"; drop_kind = kind; flags_at_drop = rtti_c::read_word(rtti_c::address(object) + 12u);
    d160_at_drop = rtti_c::read_u64(rtti_c::address(object) + 160u);
}
extern "C" void objectAddToDeleteList__FP10GameObject(void*) { log_text += "e"; }
static unsigned char table[128];
static unsigned char headers[32];
static unsigned w(unsigned off) { return rtti_c::read_word(rtti_c::address(object) + off); }
static unsigned half() { return rtti_c::read_u16(rtti_c::address(object) + 572u); }
static void reset(unsigned cur) {
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    for (unsigned i = 0; i < 32; ++i) rtti_c::host_note(headers + i);
    rtti_c::host_note(table);
    for (unsigned off : {28u, 36u, 52u, 68u}) rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(headers + off / 4));
    rtti_c::write_word(rtti_c::address(object) + 36u, rtti_c::address_bits(table));
    rtti_c::write_word(rtti_c::address(object) + 808u, 5000u);
    rtti_c::write_u16(rtti_c::address(object) + 572u, cur);
    log_text.clear(); sp_result = 0; sp_new = -1000; sp_seen = -1000; trans_n = 0; roll = 0; dest_range = 0; drop_kind = 0;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int a = 0) { if (!ok) { std::printf("fail: %s (%d)\n", msg, a); ++bad; } }
int main() {
    const unsigned tenth = 0x3dccccccu;
    reset(9); norrath_skelight_set_state(object, 0);
    expect(log_text == "pt" && sp_seen == 0 && sp_object == object && trans_header[0] == headers + 13 && trans_rate[0] == tenth && trans_flags[0] == 3 && w(256) == 5060u && half() == 0u, "0");
    reset(9); norrath_skelight_set_state(object, 1);
    expect(log_text == "pzdtg" && dest_range == 300 && trans_header[0] == headers + 17 && trans_flags[0] == 2051 && w(256) == 5300u && object[272] == 0x41 && object[283] == 0x4c && half() == 1u, "1");
    reset(9); norrath_skelight_set_state(object, 3);
    expect(log_text == "ptg" && trans_flags[0] == 2051 && w(256) == 0u && half() == 3u, "3");
    reset(9); norrath_skelight_set_state(object, 5);
    expect(log_text == "pt" && trans_flags[0] == 3 && half() == 5u, "5");
    for (int r : {0, 1, 2, 3}) {
        reset(9); roll = r; norrath_skelight_set_state(object, 6);
        expect(log_text == "ptro" && shot_header == headers + (28 + 8 * (r & 1)) / 4 && shot_a == tenth && shot_b == tenth && shot_flags == 2056 && w(256) == 1u && half() == 6u, "6", r);
    }
    reset(0x1234); norrath_skelight_set_state(object, 8);
    expect(log_text == "p" && half() == 0x1234u, "8");
    reset(9); rtti_c::write_word(rtti_c::address(object) + 12u, 1u); norrath_skelight_set_state(object, 10);
    expect(log_text == "pxe" && drop_kind == 8 && flags_at_drop == 3u && d160_at_drop == (1ull << 41) && half() == 10u, "10");
    for (int s : {2, 4, 7, 9, 11, 12, 13, 14, 20, -1}) {
        reset(9); norrath_skelight_set_state(object, s);
        expect(log_text == "p" && half() == (static_cast<unsigned>(s) & 0xffffu), "quiet states", s);
    }
    reset(9); sp_result = 1; sp_new = 5; norrath_skelight_set_state(object, 0);
    expect(log_text == "p" && half() == 9u, "nonzero processing result returns before the store");
    reset(9); sp_new = 5; norrath_skelight_set_state(object, 0);
    expect(log_text == "pt" && half() == 5u && trans_flags[0] == 3 && w(256) == 0u, "processing may replace the state (5 has no +256)");
    reset(9); sp_new = 3; norrath_skelight_set_state(object, 0);
    expect(log_text == "ptg", "replaced state selects the branch");
    std::printf(bad ? "failed\n" : "norrath_skelight_set_state passed\n");
    return bad != 0;
}
