// Host check only. Not an EE build input.
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
static int sp_result = 0, sp_new = -1000, rolls[4], roll_n = 0, roll_i = 0;
static unsigned char headers[700];
static unsigned char object[1300];
static unsigned char vtable[256];
static unsigned char table[700];
static unsigned char block[16];
static void* alloc_result = nullptr;
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned long long d(unsigned o) { return rtti_c::read_u64(a() + o); }
static unsigned half(unsigned o = 572u) { return rtti_c::read_u16(a() + o); }
static unsigned offset_of(void* h) { return static_cast<unsigned>(static_cast<unsigned char*>(h) - headers); }
struct Attach { unsigned model; short angle; int index; unsigned scale; unsigned char point[12]; };
static std::vector<Attach> attaches;
struct Prop { void* self; unsigned char point[12]; unsigned a, b, c; long d; int e; unsigned scale; unsigned w964, w968, w1112; };
static std::vector<Prop> props;
static unsigned treasure_bits[6];
static int treasure_a = 0;
static char* treasure_name = reinterpret_cast<char*>(1);
static unsigned char treasure_point[12];
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) { log_text += "p"; if (sp_new != -1000) *state = sp_new; return sp_result; }
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_i++ % roll_n]; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float rate, int flags) { log_text += "t"; calls.push_back({'t', offset_of(h), bits_of(rate), flags}); }
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a_, float b_, int flags) {
    log_text += "o"; calls.push_back({'o', offset_of(h), bits_of(a_), flags}); if (bits_of(a_) != bits_of(b_)) log_text += "!";
}
extern "C" void animGlobalSync__FP14AnimationState(void*) { log_text += "g"; }
extern "C" void resetRouting__8Creature(void*) { log_text += "z"; }
extern "C" void getSmartRandomDestination__8Creaturei(void* r, void*, int range) {
    log_text += range == 200 ? "d" : "?";
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(r)[i] = static_cast<unsigned char>(0x61 + i);
}
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* point, int a_, float f12, float f13, float f14, float f15, float f16, float f17, char* name) {
    log_text += "y"; std::memcpy(treasure_point, point, 12); treasure_a = a_; treasure_name = name;
    treasure_bits[0] = bits_of(f12); treasure_bits[1] = bits_of(f13); treasure_bits[2] = bits_of(f14); treasure_bits[3] = bits_of(f15); treasure_bits[4] = bits_of(f16); treasure_bits[5] = bits_of(f17);
}
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, const void* point, short a_, void*, int index, void* out_a, void* out_b, float scale) {
    log_text += "m"; Attach at; at.model = offset_of(model); at.angle = a_; at.index = index; at.scale = bits_of(scale); std::memcpy(at.point, point, 12); attaches.push_back(at);
    std::memset(out_a, 0x41, 12); std::memset(out_b, 0x51, 12);
}
extern "C" void* blockAlloc__Fi(int size) { log_text += size == 584 ? "a" : "?"; return alloc_result; }
extern "C" void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* point, char* a_, char* b_, char* c_, long d_, int e_, void* description, float scale) {
    log_text += "n"; Prop p; p.self = self; std::memcpy(p.point, point, 12); p.a = offset_of(a_); p.b = offset_of(b_); p.c = offset_of(c_); p.d = d_; p.e = e_; p.scale = bits_of(scale);
    p.w964 = w(964); p.w968 = w(968); p.w1112 = w(1112); props.push_back(p);
    if (description != nullptr) log_text += "!";
}
static void slot192(void*) { log_text += "s"; }
static void reset(unsigned cur, unsigned kind) {
    std::memset(object, 0, sizeof object);
    std::memset(vtable, 0, sizeof vtable);
    std::memset(table, 0, sizeof table);
    rtti_c::host_note(table);
    rtti_c::host_note(block);
    for (unsigned off : {0u, 8u, 12u, 16u, 24u, 28u, 32u, 36u, 40u, 44u, 52u, 56u, 64u, 76u, 84u, 92u, 100u, 108u, 116u, 124u, 132u, 140u, 148u, 156u, 164u, 172u, 180u, 188u, 196u, 204u, 212u, 220u, 228u, 236u, 244u, 252u,
                         260u, 268u, 276u, 284u, 292u, 300u, 308u, 324u, 340u, 356u, 416u, 424u, 512u, 520u}) {
        rtti_c::host_note(headers + off);
        rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(headers + off));
    }
    rtti_c::write_word(a() + 36u, rtti_c::address_bits(table));
    rtti_c::host_install_slot(object, vtable, 192u, 0, reinterpret_cast<void*>(slot192));
    rtti_c::write_word(a() + 808u, 5000u);
    rtti_c::write_u16(a() + 572u, cur);
    object[960] = static_cast<unsigned char>(kind);
    log_text.clear(); calls.clear(); attaches.clear(); props.clear(); sp_result = 0; sp_new = -1000; roll_n = 2; rolls[0] = 1000; rolls[1] = 61; roll_i = 0; alloc_result = block;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0, int y = 0) { if (!ok) { std::printf("fail: %s (%d, %d) log=%s\n", msg, x, y, log_text.c_str()); ++bad; } }
int main() {
    const unsigned rate = 0x3dccccccu;
    // ---- states 0 and 9 share their selection; 5 and 16 share theirs ----
    struct A { unsigned kind; int header; };
    const A a_cases[] = {{4, 76}, {5, 76}, {3, 148}, {2, 196}, {1, 252}, {0, 292}, {6, -1}, {0x80, -1}, {0xff, -1}};
    for (const A& c : a_cases) {
        for (int state : {0, 9}) {
            reset(5, c.kind); norrath_male_dark_elf_set_state(object, state);
            const bool has = c.header >= 0;
            expect(log_text == std::string("p") + (has ? "t" : "") + (state == 0 ? "r" : ""), "typed_a order", state, c.kind);
            expect(!has || (calls[0].header == static_cast<unsigned>(c.header) && calls[0].rate == rate && calls[0].flags == 3), "typed_a header", state, c.kind);
            if (state == 0) expect(w(256) == 5000u + 1000u % 60u + 120u, "state 0 +256", c.kind);
        }
    }
    struct B { unsigned kind; int header; };
    const B b_cases[] = {{2, 180}, {3, 132}, {1, 252}, {5, 52}, {4, 44}, {0, -1}, {6, -1}, {0xff, -1}};
    for (const B& c : b_cases) {
        reset(5, c.kind); norrath_male_dark_elf_set_state(object, 5);
        expect(log_text == (c.header >= 0 ? "pt" : "p") && (c.header < 0 || (calls[0].header == static_cast<unsigned>(c.header) && calls[0].flags == 3)), "typed_b", c.kind);
        reset(5, c.kind); rtti_c::write_u64(a() + 160u, 0ull); norrath_male_dark_elf_set_state(object, 16);
        expect(log_text == (c.header >= 0 ? "pt" : "p") && d(160) == 0x200ull && w(256) == 5120u, "state 16", c.kind);
    }
    // ---- state 1 ----
    struct S1 { unsigned kind; int header; };
    const S1 s1_cases[] = {{2, 220}, {3, 220}, {4, 220}, {1, 268}, {0, 300}, {5, -1}, {6, -1}, {0xff, -1}};
    for (const S1& c : s1_cases) {
        reset(5, c.kind); norrath_male_dark_elf_set_state(object, 1);
        expect(log_text == (c.header >= 0 ? "pzdtg" : "pzdg") && (c.header < 0 || (calls[0].header == static_cast<unsigned>(c.header) && calls[0].flags == 2051)) && object[272] == 0x61 && object[283] == 0x6c, "state 1", c.kind);
    }
    // ---- state 3 ----
    struct S3 { unsigned kind; int header; };
    const S3 s3_cases[] = {{2, 212}, {3, 212}, {1, 260}, {0, 300}, {4, 84}, {5, 84}, {6, -1}, {0xff, -1}, {0x84, -1}};
    for (const S3& c : s3_cases) {
        reset(5, c.kind); rtti_c::write_u64(a() + 160u, ~0ull); norrath_male_dark_elf_set_state(object, 3);
        expect(log_text == (c.header >= 0 ? "pztg" : "pzg") && (c.header < 0 || (calls[0].header == static_cast<unsigned>(c.header) && calls[0].flags == 2051)), "state 3", c.kind);
        expect(d(160) == (~0ull & ~0x200ull & ~0x8000000ull) && w(788) == 0x447a0000u, "state 3 bit 27 and +788", c.kind);
    }
    // ---- state 7 ----
    reset(5, 1); norrath_male_dark_elf_set_state(object, 7); expect(log_text == "pzt" && calls[0].header == 212 && calls[0].flags == 2051 && w(256) == 5120u, "state 7");
    // ---- state 6 ----
    struct S6 { unsigned kind; int t, o; };
    const S6 s6_cases[] = {{2, 180, 156 + 8 * (1000 % 3)}, {3, 132, 108 + 8 * (1000 % 3)}, {1, 252, 228 + 8 * (1000 & 1)}, {5, 52, 28 + 8 * (1000 & 1)}, {4, 44, 28 + 8 * (1000 & 1)}};
    for (const S6& c : s6_cases) {
        reset(5, c.kind); norrath_male_dark_elf_set_state(object, 6);
        expect(log_text == "ptror" + std::string("") && calls[0].header == static_cast<unsigned>(c.t) && calls[1].header == static_cast<unsigned>(c.o) && calls[1].flags == 2056 && calls[0].flags == 3, "state 6", c.kind);
        expect(w(256) == 61u % 20u, "state 6 +256", c.kind);
    }
    reset(5, 0); norrath_male_dark_elf_set_state(object, 6); expect(log_text == "pr" && w(256) == 1000u % 20u, "state 6 with kind 0");
    // ---- 13 14 15 ----
    reset(5, 0); norrath_male_dark_elf_set_state(object, 15); expect(log_text == "ptor" && calls[0].header == 292 && calls[1].header == 276 && calls[1].flags == 2056 && w(256) == 1000u % 15u, "state 15");
    reset(5, 0); norrath_male_dark_elf_set_state(object, 13); expect(log_text == "ptor" && calls[0].header == 76 && calls[1].header == 100 && w(256) == 1000u % 15u, "state 13");
    reset(5, 0); norrath_male_dark_elf_set_state(object, 14); expect(log_text == "ptor" && calls[0].header == 76 && calls[1].header == 92, "state 14");
    // ---- 8 ----
    reset(9, 0); norrath_male_dark_elf_set_state(object, 8); expect(log_text == "p" && half() == 9u, "8 current 9");
    reset(6, 0); rtti_c::write_u16(a() + 294u, 8u); norrath_male_dark_elf_set_state(object, 8); expect(log_text == "p" && half() == 6u, "8 current 6 bit 8");
    reset(14, 1); rtti_c::write_u16(a() + 294u, 8u); norrath_male_dark_elf_set_state(object, 8); expect(log_text == "po" && calls[0].header == 244, "8 current 14 is not special");
    for (const B& c : std::initializer_list<B>{{1, 244}, {0, 284}, {2, 204}, {3, 204}, {0xff, 204}}) {
        reset(6, c.kind); norrath_male_dark_elf_set_state(object, 8);
        expect(log_text == "po" && calls[0].header == static_cast<unsigned>(c.header) && calls[0].flags == 16 && calls[0].rate == rate && half() == 8u, "8 one-shot", c.kind);
    }
    // ---- 10 ----
    reset(5, 0); rtti_c::write_u16(a() + 56u, 0xfffeu); for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x20 + i);
    rtti_c::write_word(a() + 12u, 2u); norrath_male_dark_elf_set_state(object, 10);
    expect(log_text == "prtys" && calls[0].header == 308 + 16 * (1000 & 3) && calls[0].flags == 6 && treasure_a == -2 && treasure_name == nullptr, "10 with the item bit");
    expect(treasure_bits[0] == 0x42100000u && treasure_bits[1] == 0x40000000u && treasure_bits[2] == 0x3fe66666u && treasure_bits[3] == 0x3f199999u && treasure_bits[4] == 0x40000000u && treasure_bits[5] == 0x3e800000u, "10 treasure floats");
    reset(5, 0); rtti_c::write_u16(a() + 56u, 0x4001u); for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x20 + i);
    rtti_c::write_word(a() + 964u, 52u); rtti_c::write_word(a() + 968u, 3u); rtti_c::write_word(a() + 1112u, 4u); rtti_c::write_word(a() + 224u, 0x3f000000u);
    norrath_male_dark_elf_set_state(object, 10);
    expect(log_text == "prtymanmanmans" + std::string("") || log_text == "prtymanmanmans", "10 spawns three props");
    expect(attaches.size() == 3 && attaches[0].index == 0 && attaches[1].index == 1 && attaches[2].index == 2 && attaches[0].model == 12 && attaches[0].angle == 1 && attaches[0].scale == 0x3f800000u && std::memcmp(attaches[0].point, object + 16, 12) == 0, "10 attachment calls");
    expect(props.size() == 3 && props[0].a == 0 && props[0].b == 52 * 8 && props[0].c == 52 * 8 + 8 && props[0].d == 1 && props[0].e == 8193 && props[0].scale == 0x3f000000u, "10 first prop");
    expect(props[1].b == 3 * 8 && props[1].c == 3 * 8 + 8 && props[1].e == 16385 && props[2].b == 4 * 8 && props[2].e == 16385, "10 second and third props");
    expect(props[0].w964 == 52u && props[1].w968 == 3u && props[2].w1112 == 4u && w(964) == 0u && w(968) == 0u && w(1112) == 0u, "10 clears the words after each prop");
    expect(props[0].point[0] == 0x41 && props[0].point[11] == 0x41, "10 prop position is the first output");
    reset(5, 0); rtti_c::write_word(a() + 964u, 64u); norrath_male_dark_elf_set_state(object, 10); expect(props.size() == 1 && props[0].e == 8193, "10 value 64");
    reset(5, 0); rtti_c::write_word(a() + 964u, 7u); norrath_male_dark_elf_set_state(object, 10); expect(props.size() == 1 && props[0].e == 16385, "10 other value");
    reset(5, 0); alloc_result = nullptr; rtti_c::write_word(a() + 964u, 7u); norrath_male_dark_elf_set_state(object, 10);
    expect(log_text == "prtymas" && props.empty() && w(964) == 0u, "10 allocation failure still clears the word");
    // ---- quiet states and early returns ----
    for (int s : {2, 4, 11, 12, 17, 99, -1}) { reset(9, 1); norrath_male_dark_elf_set_state(object, s); expect(log_text == "p" && half() == (static_cast<unsigned>(s) & 0xffffu), "quiet states", s); }
    reset(9, 1); sp_result = 1; sp_new = 5; rtti_c::write_u64(a() + 160u, 0x200ull); norrath_male_dark_elf_set_state(object, 0); expect(log_text == "p" && half() == 9u && d(160) == 0x200ull, "processing result returns before the flag clear");
    reset(9, 1); sp_new = 7; norrath_male_dark_elf_set_state(object, 0); expect(log_text == "pzt" && half() == 7u, "processing may replace the state");
    std::printf(bad ? "failed\n" : "norrath_male_dark_elf_set_state passed\n");
    return bad != 0;
}
