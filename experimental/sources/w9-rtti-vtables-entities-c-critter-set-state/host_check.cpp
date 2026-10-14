// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "set_state.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static char log_text[32];
static unsigned log_n = 0;
static int rolls[3], roll_i = 0;
static int path_result = 0;
static int dest_range = 0;
static void* trans_header = nullptr;
static unsigned trans_rate = 0;
static int trans_flags = 0;
static void* trans_state = nullptr;
static unsigned w256_at_trans = 0, w1032_at_trans = 0;
static char path_seen[8];
static void note(char c) { log_text[log_n++] = c; log_text[log_n] = 0; }
extern "C" int eRand__10GameObject(void*) { note('r'); return rolls[roll_i++ % 3]; }
extern "C" char* findTagString__10GameObjectPci(void*, char* tag, int) {
    note('f'); std::strncpy(path_seen, tag, 7); return path_result ? tag : nullptr;
}
extern "C" void resetRouting__8Creature(void*) { note('z'); }
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void*, int range) {
    note('d'); dest_range = range;
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(result)[i] = static_cast<unsigned char>(0x31 + i);
}
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float rate, int flags) {
    note('t'); trans_state = s; trans_header = h; std::memcpy(&trans_rate, &rate, 4); trans_flags = flags;
    w256_at_trans = rtti_c::read_word(rtti_c::address(s) - 288u + 256u);
    w1032_at_trans = rtti_c::read_word(rtti_c::address(s) - 288u + 1032u);
}
static unsigned char object[1300];
static unsigned char headers[8];
static unsigned w(unsigned off) { return rtti_c::read_word(rtti_c::address(object) + off); }
static void put(unsigned off, unsigned v) { rtti_c::write_word(rtti_c::address(object) + off, v); }
static void reset(unsigned cur) {
    std::memset(object, 0, sizeof object);
    for (int i = 0; i < 8; ++i) rtti_c::host_note(headers + i);
    put(808u, 1000u); put(1020u, 20u); put(1024u, 30u); put(1028u, 40u);
    put(1044u, rtti_c::address_bits(headers)); put(1056u, rtti_c::address_bits(headers + 1)); put(1060u, rtti_c::address_bits(headers + 2));
    put(1064u, rtti_c::address_bits(headers + 3)); put(1068u, rtti_c::address_bits(headers + 4)); put(1072u, rtti_c::address_bits(headers + 5));
    rtti_c::write_u16(rtti_c::address(object) + 572u, cur);
    rolls[0] = 1234; rolls[1] = 777; roll_i = 0; log_n = 0; log_text[0] = 0; path_result = 0; dest_range = 0;
    trans_header = nullptr; trans_state = nullptr; trans_rate = 0; trans_flags = 0;
}
static unsigned half() { return rtti_c::read_u16(rtti_c::address(object) + 572u); }
static int bad = 0;
static void expect(bool ok, const char* msg, int a = 0) { if (!ok) { std::printf("fail: %s (%d)\n", msg, a); ++bad; } }
int main() {
    // state 0
    reset(5); setState__7Critteri(object, 0);
    expect(std::strcmp(log_text, "frrt") == 0, "0 order");
    expect(w(256u) == 1000u + 30u + 1234u % 120u, "0 +256");
    expect(w1032_at_trans == 1000u + 40u + 777u % 60u && w(1032u) == w1032_at_trans, "0 +1032 set before call");
    expect(trans_header == headers && trans_rate == 0x3e999999u && trans_flags == 3 && trans_state == object + 288, "0 transition");
    expect(half() == 0u, "0 halfword");
    // state 1 with the long header
    reset(5); put(160u, 0x400000u); setState__7Critteri(object, 1);
    expect(std::strcmp(log_text, "frzdt") == 0, "1 order");
    expect(w(256u) == 1000u + 20u + 1234u % 120u && dest_range == 200, "1 +256 and range");
    for (int i = 0; i < 12; ++i) expect(object[272 + i] == 0x31 + i, "1 point copy", i);
    expect(trans_header == headers + 2 && trans_rate == 0x3dccccccu && trans_flags == 2051, "1 header 1060");
    reset(5); setState__7Critteri(object, 1);
    expect(trans_header == headers + 1, "1 header 1056 without flag bits");
    reset(5); put(160u, 0x800000u); put(1060u, 0u); setState__7Critteri(object, 1);
    expect(trans_header == headers + 1, "1 header 1056 when 1060 is null");
    // state 7
    reset(5); put(1064u, 0u); setState__7Critteri(object, 7);
    expect(log_text[1] == 0 && half() == 5u && w(256u) == 0u, "7 without header keeps the current state");
    reset(5); setState__7Critteri(object, 7);
    expect(std::strcmp(log_text, "frzt") == 0 && w(256u) == 1000u + 60u + 1234u % 120u && trans_header == headers + 3 && trans_rate == 0x3c23d70au, "7 with header");
    // 12 and the path tag
    reset(5); setState__7Critteri(object, 12);
    expect(std::strcmp(log_text, "t") == 0 && trans_header == headers + 1 && trans_flags == 2051, "12");
    reset(5); path_result = 1; setState__7Critteri(object, 3);
    expect(std::strcmp(log_text, "ft") == 0 && half() == 12u && std::strcmp(path_seen, "path") == 0, "path forces 12");
    // 13 and 15
    reset(5); setState__7Critteri(object, 13);
    expect(std::strcmp(log_text, "frt") == 0 && w(256u) == 1300u && w(1032u) == 1000u + 40u + 1234u % 60u && trans_header == headers + 4 && trans_rate == 0x3e999999u && trans_flags == 3, "13");
    reset(5); put(1076u, 1u); put(1080u, 99u); setState__7Critteri(object, 15);
    expect(w(1080u) == 0u && half() == 15u, "15 clears +1080 when bit 1 is set");
    reset(5); put(1076u, 2u); put(1080u, 99u); setState__7Critteri(object, 15);
    expect(w(1080u) == 99u, "15 keeps +1080 otherwise");
    // 14 and 16
    for (int s : {14, 16}) {
        reset(5); setState__7Critteri(object, s);
        expect(std::strcmp(log_text, "ft") == 0 && w(256u) == 1000u && (w(12u) & 1u) == 1u && trans_header == headers + 5 && trans_rate == 0x3c23d70au && half() == static_cast<unsigned>(s), "14/16", s);
    }
    // quiet states and out-of-range requests keep the current halfword
    for (int s : {2, 3, 4, 5, 6, 8, 9, 10, 11, 17, 18, 100, -1}) {
        reset(9); setState__7Critteri(object, s);
        expect(half() == 9u && std::strcmp(log_text, "f") == 0, "quiet state", s);
    }
    // current-state overrides
    reset(13); setState__7Critteri(object, 3); expect(half() == 13u && std::strcmp(log_text, "frt") == 0 && trans_rate == 0x3e999999u, "cur 13 forces 13");
    reset(13); setState__7Critteri(object, 14); expect(half() == 14u, "cur 13 allows 14");
    reset(13); setState__7Critteri(object, 13); expect(half() == 13u, "cur 13 allows 13");
    reset(14); setState__7Critteri(object, 2); expect(half() == 14u, "cur 14 forces 14");
    reset(15); setState__7Critteri(object, 2); expect(half() == 15u, "cur 15 forces 15");
    reset(15); setState__7Critteri(object, 16); expect(half() == 16u, "cur 15 allows 16");
    reset(16); setState__7Critteri(object, 2); expect(half() == 16u, "cur 16 forces 16");
    reset(16); setState__7Critteri(object, 15); expect(half() == 15u, "cur 16 allows 15");
    std::printf(bad ? "failed\n" : "setState__7Critteri passed\n");
    return bad != 0;
}
