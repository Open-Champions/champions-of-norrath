// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "init_type.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
unsigned char skills[252 + 64];
static int rolls = 0, call_result = 0, calls = 0, params_flag = -9, params_calls = 0;
static unsigned params_bits[5];
static void* call_self = nullptr;
static unsigned char object[1300];
static unsigned char owners[4][128], tables[4][128];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" int eRand__10GameObject(void*) { ++rolls; return 0; }
extern "C" void setParameters__8Creaturefffffi(void*, float a, float b, float c, float d, float e, int flag) {
    ++params_calls; params_flag = flag;
    params_bits[0] = bits_of(a); params_bits[1] = bits_of(b); params_bits[2] = bits_of(c); params_bits[3] = bits_of(d); params_bits[4] = bits_of(e);
}
static int vslot(void* self, int, int) { ++calls; call_self = self; return call_result; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned h(unsigned o) { return rtti_c::read_u16(a() + o); }
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0) { if (!ok) { std::printf("fail: %s (%d)\n", msg, x); ++bad; } }
static void setup(unsigned variant, unsigned kind) {
    std::memset(object, 0x5a, sizeof object);
    rtti_c::write_word(a() + 208u, variant); rtti_c::write_word(a() + 960u, kind);
    for (int k = 0; k < 4; ++k) {
        std::memset(owners[k], 0, sizeof owners[k]); std::memset(tables[k], 0, sizeof tables[k]);
        rtti_c::host_note(owners[k]); rtti_c::host_note(tables[k]);
        rtti_c::write_word(rtti_c::address(owners[k]) + 96u, rtti_c::address_bits(tables[k]));
        rtti_c::write_word(rtti_c::address(tables[k]) + 100u, rtti_c::address_bits(reinterpret_cast<void*>(vslot)));
    }
    rtti_c::host_note(reinterpret_cast<void*>(vslot));
    rtti_c::write_word(rtti_c::address(skills) + 52u, rtti_c::address_bits(owners[0]));   // 13
    rtti_c::write_word(rtti_c::address(skills) + 56u, rtti_c::address_bits(owners[1]));   // 14
    rtti_c::write_word(rtti_c::address(skills) + 100u, rtti_c::address_bits(owners[2]));  // 25
    calls = 0; rolls = 0; params_calls = 0; call_result = 40; call_self = nullptr;
}
int main() {
    struct P { unsigned kind, bits[5]; bool b; };
    const P parameters[] = {
        {0, {0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u}, false},
        {1, {0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u}, true},
        {2, {0x3f999999u, 0x3f999999u, 0x3f999999u, 0x3f999999u, 0x3f999999u}, false},
        {3, {0x3f4cccccu, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u}, true},
        {4, {0x3fc00000u, 0x3fa66666u, 0x3fb33333u, 0x3f999999u, 0x3fa66666u}, false},
        {5, {0x3fa66666u, 0x3f800000u, 0x3f800000u, 0x3f999999u, 0x3f999999u}, true}};
    for (const P& p : parameters) {
        setup(0, p.kind); norrath_mummy_init_type(object, 3);
        expect(params_calls == 1 && params_flag == 2, "flag xor", p.kind);
        expect(std::memcmp(params_bits, p.bits, sizeof params_bits) == 0, "parameters", p.kind);
        expect(w(88) == 0xbe4cccccu && w(100) == 0x3f000000u && w(96) == 0x3e999999u, "store set A", p.kind);
        expect(p.b == (w(104) == 0x3e4cccccu), "store set B word", p.kind);
        expect(object[156] == (p.b ? 6 : 0), "byte 156", p.kind);
        expect(h(1068) == 0xffffu && h(1076) == 0xffffu && h(1084) == 0xffffu && h(1092) == 0xffffu, "slots cleared", p.kind);
    }
    for (unsigned kind : {6u, 7u, 0xffffffffu}) {
        setup(0, kind); norrath_mummy_init_type(object, 0);
        expect(params_calls == 0 && w(88) == 0x5a5a5a5au && object[156] == 0, "kind outside the table", kind);
    }
    setup(0, 0); norrath_mummy_init_type(object, 0); expect(params_flag == 1, "flag 0 gives 1");
    struct S { unsigned variant, kind, byte; int owner; unsigned type; };
    const S slots[] = {{1, 0, 7, 2, 38}, {1, 5, 7, 2, 38}, {2, 2, 3, 1, 22}, {2, 0, 2, 0, 21}, {2, 4, 2, 0, 21}, {3, 0, 8, -1, 0}, {4, 7, 8, -1, 0}, {5, 7, 0, -1, 0}};
    for (const S& s : slots) {
        setup(s.variant, s.kind); norrath_mummy_init_type(object, 0);
        expect(object[156] == s.byte, "final byte", s.variant * 10 + s.kind);
        if (s.owner < 0) expect(calls == 0 && h(1068) == 0xffffu, "no slot work", s.variant * 10 + s.kind);
        else expect(calls == 1 && call_self == owners[s.owner] && h(1068) == s.type && h(1070) == 40u && h(1074) == 0u && rolls == 1, "slot", s.variant * 10 + s.kind);
    }
    setup(4, 7); norrath_mummy_init_type(object, 0); expect(w(212) == 1u, "variant 4 sets +212");
    setup(3, 7); norrath_mummy_init_type(object, 0); expect(w(212) == 0x5a5a5a5au, "variant 3 leaves +212");
    std::printf(bad ? "failed\n" : "norrath_mummy_init_type passed\n");
    return bad != 0;
}
