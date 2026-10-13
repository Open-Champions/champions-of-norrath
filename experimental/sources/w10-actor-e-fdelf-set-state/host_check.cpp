// Host contract for setState__13FemaleDarkElfi. Includes the source; not an EE input.
#define NORRATH_RTTI_C_HOST 1
#include "set_state.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
struct Call { char kind; unsigned header; unsigned rate; int flags; };
static std::vector<Call> calls;
static std::string log_text;
static int sp_result = 0, sp_new = -1000, rolls[4], roll_n = 0, roll_i = 0, range_seen = 0, init_flag = -1, attach_index[2], attach_n = 0, alloc_n = 0;
static void* alloc_result = nullptr;
static unsigned char headers[400], object[1300], vtable[256], table[400], block[600];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned long long d(unsigned o) { return rtti_c::read_u64(a() + o); }
static unsigned half(unsigned o = 572u) { return rtti_c::read_u16(a() + o); }
static unsigned offset_of(void* h) { return static_cast<unsigned>(static_cast<unsigned char*>(h) - headers); }
static unsigned treasure_bits[6]; static int treasure_a = 0; static char* treasure_name = reinterpret_cast<char*>(1);
static unsigned char point_copy[12]; static unsigned w256_at_call = 0, init_w964 = 0, init_w1104 = 0; static unsigned long long init_flags = 0;
struct Prop { void* self; unsigned char at[12]; unsigned a, b, c; long d; int e; void* desc; unsigned scale; };
static std::vector<Prop> props;
static unsigned angle_seen[2]; static float scale_seen[2];
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) { log_text += "p"; if (sp_new != -1000) *state = sp_new; return sp_result; }
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_i++ % roll_n]; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float rate, int flags) {
    log_text += "t"; w256_at_call = w(256); calls.push_back({'t', offset_of(h), bits_of(rate), flags});
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a_, float b_, int flags) {
    log_text += "o"; calls.push_back({'o', offset_of(h), bits_of(a_), flags}); if (bits_of(a_) != bits_of(b_)) log_text += "!";
}
extern "C" void animAddToEnd__FP14AnimationStateP15AnimationHeaderi(void*, void* h, int flags) { log_text += "e"; calls.push_back({'e', offset_of(h), 0, flags}); }
extern "C" void animGlobalSync__FP14AnimationState(void*) { log_text += "g"; }
extern "C" void resetRouting__8Creature(void*) { log_text += "z"; }
extern "C" void getSmartRandomDestination__8Creaturei(void* r, void*, int range) {
    log_text += "d"; range_seen = range;
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(r)[i] = static_cast<unsigned char>(0x61 + i);
}
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* point, int a_, float f12, float f13, float f14, float f15, float f16, float f17, char* name) {
    log_text += "y"; std::memcpy(point_copy, point, 12); treasure_a = a_; treasure_name = name;
    treasure_bits[0] = bits_of(f12); treasure_bits[1] = bits_of(f13); treasure_bits[2] = bits_of(f14); treasure_bits[3] = bits_of(f15); treasure_bits[4] = bits_of(f16); treasure_bits[5] = bits_of(f17);
}
extern "C" void initType__13FemaleDarkElfb(void*, bool flag) {
    log_text += "i"; init_flag = flag; init_flags = d(160); init_w964 = w(964); init_w1104 = w(1104);
}
extern "C" void objectAddToRunList__FP10GameObject(void*) { log_text += "u"; }
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* point, short a_, void*, int index, void* out_a, void* out_b, float scale) {
    log_text += "m"; (void)model; (void)point;
    angle_seen[attach_n & 1] = static_cast<unsigned short>(a_); scale_seen[attach_n & 1] = scale; attach_index[attach_n & 1] = index; ++attach_n;
    for (int i = 0; i < 3; ++i) { static_cast<float*>(out_a)[i] = 100.0f + static_cast<float>(index); static_cast<float*>(out_b)[i] = 9.0f; }
}
extern "C" void* blockAlloc__Fi(int size) { log_text += "b"; ++alloc_n; if (size != 584) log_text += "!"; return alloc_result; }
extern "C" void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* point, char* a_, char* b_, char* c_, long d_, int e_, void* desc, float scale) {
    log_text += "P"; Prop p{self, {}, static_cast<unsigned>(reinterpret_cast<unsigned long long>(a_) - 0), 0, 0, d_, e_, desc, bits_of(scale)};
    std::memcpy(p.at, point, 12);
    p.a = static_cast<unsigned>(a_ - reinterpret_cast<char*>(headers)); p.b = static_cast<unsigned>(b_ - reinterpret_cast<char*>(headers)); p.c = static_cast<unsigned>(c_ - reinterpret_cast<char*>(headers));
    props.push_back(p);
}
static void* slot_self = nullptr;
static void slot192(void* self) { log_text += "s"; slot_self = self; }
static void reset(unsigned cur, int kind = 0) {
    std::memset(object, 0, sizeof object); std::memset(vtable, 0, sizeof vtable); std::memset(table, 0, sizeof table);
    rtti_c::host_note(table);
    for (unsigned off : {0u, 12u, 32u, 36u, 40u, 44u, 48u, 52u, 56u, 60u, 68u, 76u, 84u, 92u, 108u, 116u, 124u, 132u, 140u, 148u, 156u, 164u, 172u, 180u, 188u, 204u, 212u, 220u, 228u}) {
        rtti_c::host_note(headers + off);
        rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(headers + off));
    }
    rtti_c::write_word(a() + 36u, rtti_c::address_bits(table));
    rtti_c::host_install_slot(object, vtable, 192u, -8, reinterpret_cast<void*>(slot192));
    rtti_c::write_word(a() + 808u, 5000u);
    rtti_c::write_u16(a() + 572u, cur);
    object[960] = static_cast<unsigned char>(kind);
    rtti_c::write_single(a() + 224u, 2.5f);
    log_text.clear(); calls.clear(); props.clear(); sp_result = 0; sp_new = -1000; roll_n = 2; rolls[0] = 1000; rolls[1] = 61; roll_i = 0; slot_self = nullptr;
    attach_n = 0; alloc_n = 0; alloc_result = block; init_flag = -1;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0, int y = 0) { if (!ok) { std::printf("fail: %s (%d, %d) log=%s\n", msg, x, y, log_text.c_str()); ++bad; } }
int main() {
    const unsigned fast = 0x3dccccccu;
    struct K { int state; const char* log; unsigned header; int flags; };
    const K simple[] = {{5, "pt", 180, 3}, {9, "pt", 212, 3}, {0, "pt", 212, 3}};
    for (const K& c : simple) {
        reset(5); setState__13FemaleDarkElfi(object, c.state);
        expect(log_text == c.log && calls.size() == 1 && calls[0].header == c.header && calls[0].rate == fast && calls[0].flags == c.flags && half() == unsigned(c.state), "simple", c.state);
    }
    reset(5); setState__13FemaleDarkElfi(object, 0); expect(w(256) == 5180u && w256_at_call == 0u, "0 sets +256 after the call");
    reset(5); setState__13FemaleDarkElfi(object, 1);
    expect(log_text == "pzdtg" && range_seen == 200 && object[272] == 0x61 && object[283] == 0x6c && calls[0].header == 228 && calls[0].flags == 2051 && w(256) == 0u, "1");
    for (int kind : {0, 1, 2, 255}) { reset(5, kind); setState__13FemaleDarkElfi(object, 3); expect(log_text == "pztg" && calls[0].header == (kind == 1 ? 228u : 220u) && calls[0].flags == 2051, "3", kind); }
    reset(5); setState__13FemaleDarkElfi(object, 7); expect(log_text == "pztg" && calls[0].header == 140 && calls[0].flags == 2051 && w(256) == 5120u, "7");
    reset(5); rolls[0] = 1001; rolls[1] = 123; setState__13FemaleDarkElfi(object, 6);
    expect(log_text == "ptror" && calls[0].header == 180 && calls[1].header == 148 + 8 * (1001 & 3) && calls[1].flags == 2056 && w(256) == 123u % 60u, "6");
    reset(5); rolls[0] = 1001; rolls[1] = 123; setState__13FemaleDarkElfi(object, 13);
    expect(log_text == "ptror" + std::string("") && calls[0].header == 132 && calls[1].header == 108 + 8 * (1001 & 1) && w(256) == 123u % 60u + 120u, "13");
    reset(5); setState__13FemaleDarkElfi(object, 14);
    expect(log_text == "pt" && calls[0].header == 180 && w(256) == 5120u && (d(160) & 0x200ull) != 0ull, "14");
    for (unsigned long long start : {0ull, 0x200ull, ~0ull}) {
        reset(5); rtti_c::write_u64(a() + 160u, start); setState__13FemaleDarkElfi(object, 5); expect(d(160) == (start & ~0x200ull), "bit 9 cleared on entry", int(start & 0xff));
        reset(5); rtti_c::write_u64(a() + 160u, start); setState__13FemaleDarkElfi(object, 99); expect(d(160) == (start & ~0x200ull), "bit 9 cleared when only storing", int(start & 0xff));
    }
    reset(9); setState__13FemaleDarkElfi(object, 8); expect(log_text == "p" && half() == 9u, "8 keeps 9");
    for (unsigned w744 : {0x80000u, 0x100000u, 0x200000u, 0x380000u}) {
        reset(5); rtti_c::write_word(a() + 744u, w744); rolls[0] = 6; setState__13FemaleDarkElfi(object, 8);
        const unsigned k = 32u + 16u * (6u & 3u);
        expect(log_text == "prte" + std::string("e") && calls[0].header == k + 4 && calls[0].flags == 20 && calls[1].header == k + 12 && calls[1].flags == 36 && calls[2].header == 212 && calls[2].flags == 2, "8 with +744 bits", w744);
    }
    reset(5); rtti_c::write_word(a() + 744u, 0x400000u); setState__13FemaleDarkElfi(object, 8); expect(log_text == "po" && calls[0].header == 204 && calls[0].flags == 16 && half() == 8u, "8 kind 0");
    reset(5, 1); setState__13FemaleDarkElfi(object, 8); expect(log_text == "po" && calls[0].header == 124 && calls[0].flags == 16, "8 kind 1");
    reset(5, 2); setState__13FemaleDarkElfi(object, 8); expect(log_text == "po" && calls[0].header == 204, "8 kind 2");
    // 15
    reset(5); rtti_c::write_word(a() + 12u, 0xffffffffu); rolls[0] = 7; rolls[1] = 8; setState__13FemaleDarkElfi(object, 15);
    expect(log_text == "prriu" && w(12) == 0xfffffff6u && w(964) == (7u & 2u) + 29u && w(1104) == (8u % 3u) * 2u + 33u && init_flag == 0 && init_w964 == w(964) && init_w1104 == w(1104) &&
           init_flags == (1ull << 32) && d(160) == (1ull << 32), "15 kind 0");
    expect(half(146) == 50u && half(150) == 128u && half(152) == 255u && half(154) == 150u && half(140) == 0u && half(142) == 128u && half(144) == 255u && half(148) == 0u && half() == 0u, "15 halfwords and state");
    reset(5, 1); rtti_c::write_word(a() + 964u, 7u); rtti_c::write_word(a() + 1104u, 9u); setState__13FemaleDarkElfi(object, 15);
    expect(log_text == "piu" && w(964) == 7u && w(1104) == 9u, "15 other kinds keep the weapon words");
    // 10
    reset(5); for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0xa0 + i);
    rtti_c::write_u16(a() + 56u, 0xfff0u); rolls[0] = 6; setState__13FemaleDarkElfi(object, 10);
    expect(log_text == "prtys" && calls[0].header == 36 + 16 * (6 & 3) && calls[0].flags == 6 && treasure_a == -16 && treasure_name == nullptr &&
           std::memcmp(point_copy, object + 16, 12) == 0 && slot_self == object - 8, "10 plain");
    expect(treasure_bits[0] == 0x42100000u && treasure_bits[1] == 0x40000000u && treasure_bits[2] == 0x3fe66666u && treasure_bits[3] == 0x3f199999u &&
           treasure_bits[4] == 0x40000000u && treasure_bits[5] == 0x3e800000u, "10 treasure floats");
    reset(5); rtti_c::write_u64(a() + 160u, 1ull << 32); setState__13FemaleDarkElfi(object, 10); expect(log_text == "prts", "10 bit 32 skips the treasure");
    reset(5); rtti_c::write_word(a() + 12u, 2u); rtti_c::write_word(a() + 964u, 4u); setState__13FemaleDarkElfi(object, 10); expect(log_text == "prtys" && w(964) == 4u, "10 bit 1 of +12 skips the props");
    reset(5); rtti_c::write_word(a() + 964u, 4u); rtti_c::write_word(a() + 1104u, 6u); rtti_c::write_u16(a() + 56u, 20000u);
    for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0xb0 + i);
    setState__13FemaleDarkElfi(object, 10);
    expect(log_text == "prtymbPmbPs" && attach_index[0] == 0 && attach_index[1] == 1 && angle_seen[0] == 20000u - 16384u && scale_seen[0] == 1.0f && w(964) == 0u && w(1104) == 0u, "10 two props");
    expect(props.size() == 2 && props[0].e == 16641 && props[0].d == 1 && props[0].desc == nullptr && props[0].scale == bits_of(2.5f) && props[0].a == 0 && props[0].b == 32 && props[0].c == 40 &&
           props[1].b == 48 && props[1].c == 56 && props[0].self == block, "10 prop arguments");
    reset(5); rtti_c::write_word(a() + 964u, 4u); alloc_result = nullptr; setState__13FemaleDarkElfi(object, 10); expect(log_text == "prtymbs" && w(964) == 0u && props.empty(), "10 failed allocation still clears the word");
    reset(5); rtti_c::write_word(a() + 1104u, 6u); setState__13FemaleDarkElfi(object, 10); expect(log_text == "prtymbPs" && w(1104) == 0u && attach_index[0] == 1, "10 only the second word");
    for (int st : {2, 4, 11, 12, 16, 100, -1}) { reset(5); setState__13FemaleDarkElfi(object, st); expect(log_text == "p" && half() == (unsigned(st) & 0xffffu), "store only", st); }
    reset(5); sp_result = 1; sp_new = 9; rtti_c::write_u64(a() + 160u, 0x200ull); setState__13FemaleDarkElfi(object, 5);
    expect(log_text == "p" && half() == 5u && d(160) == 0x200ull, "processing result stops everything");
    reset(5); sp_new = 9; setState__13FemaleDarkElfi(object, 3); expect(log_text == "pt" && calls[0].header == 212 && half() == 9u, "processing can replace the state");
    return bad ? 1 : 0;
}
