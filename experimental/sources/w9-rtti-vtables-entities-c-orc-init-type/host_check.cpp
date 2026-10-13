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
static unsigned long long d(unsigned o) { return rtti_c::read_u64(a() + o); }
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0) { if (!ok) { std::printf("fail: %s (%d)\n", msg, x); ++bad; } }
static void setup(unsigned variant, unsigned kind) {
    std::memset(object, 0x5a, sizeof object);
    rtti_c::write_word(a() + 208u, variant); rtti_c::write_word(a() + 1116u, kind);
    rtti_c::write_u64(a() + 160u, 0xffffffffffffffffull);
    for (int k = 0; k < 4; ++k) {
        std::memset(owners[k], 0, sizeof owners[k]); std::memset(tables[k], 0, sizeof tables[k]);
        rtti_c::host_note(owners[k]); rtti_c::host_note(tables[k]);
        rtti_c::write_word(rtti_c::address(owners[k]) + 96u, rtti_c::address_bits(tables[k]));
        rtti_c::write_word(rtti_c::address(tables[k]) + 100u, rtti_c::address_bits(reinterpret_cast<void*>(vslot)));
    }
    rtti_c::host_note(reinterpret_cast<void*>(vslot));
    rtti_c::write_word(rtti_c::address(skills) + 48u, rtti_c::address_bits(owners[0]));   // 12
    rtti_c::write_word(rtti_c::address(skills) + 52u, rtti_c::address_bits(owners[1]));   // 13
    rtti_c::write_word(rtti_c::address(skills) + 56u, rtti_c::address_bits(owners[2]));   // 14
    rtti_c::write_word(rtti_c::address(skills) + 104u, rtti_c::address_bits(owners[3]));  // 26
    calls = 0; rolls = 0; params_calls = 0; call_result = 40; call_self = nullptr;
}
int main() {
    const unsigned long long keep = 0xfffffff1ffffffffull;
    struct P { unsigned kind, bits[5]; unsigned bit; };
    const P parameters[] = {
        {0, {0x3f8cccccu, 0x3f4cccccu, 0x3f8cccccu, 0x3f4cccccu, 0x3f4cccccu}, 33}, {4, {0x3f8cccccu, 0x3f4cccccu, 0x3f8cccccu, 0x3f4cccccu, 0x3f4cccccu}, 33},
        {1, {0x40000000u, 0x40000000u, 0x3fa66666u, 0x40000000u, 0x40000000u}, 35},
        {2, {0x3fe66666u, 0x3f999999u, 0x3fa66666u, 0x3f999999u, 0x3f999999u}, 35},
        {3, {0x40266666u, 0x3f999999u, 0x3fa66666u, 0x3f999999u, 0x3fb33333u}, 35},
        {5, {0x3fa66666u, 0x3f800000u, 0x3fa66666u, 0x3f800000u, 0x3f800000u}, 34}};
    for (const P& p : parameters) {
        setup(0, p.kind); norrath_orc_init_type(object, 3);
        expect(params_calls == 1 && params_flag == 2, "flag xor", p.kind);
        expect(std::memcmp(params_bits, p.bits, sizeof params_bits) == 0, "parameters", p.kind);
        expect(d(160) == (keep | (1ull << p.bit)), "bit", p.kind);
        expect(object[156] == (p.kind == 5 ? 6 : 0), "byte 156", p.kind);
        expect((p.kind == 3) == (w(28) == 0x41b00000u && w(224) == 0x3fa66666u), "kind 3 singles", p.kind);
        expect(h(1076) == 0xffffu && h(1084) == 0xffffu && h(1092) == 0xffffu && h(1100) == 0xffffu, "slots cleared", p.kind);
    }
    setup(0, 6); norrath_orc_init_type(object, 0);
    expect(params_calls == 0 && d(160) == keep && object[156] == 0, "unmatched kind");
    setup(0, 0); norrath_orc_init_type(object, 0); expect(params_flag == 1, "flag 0 gives 1");
    struct S { unsigned variant, kind, byte; int owner; unsigned type; };
    const S slots[] = {{1, 4, 3, -1, 0}, {1, 0, 1, 0, 24}, {1, 3, 1, 0, 24}, {2, 4, 2, -1, 0}, {2, 1, 3, 2, 22}, {3, 4, 2, -1, 0}, {3, 2, 2, 1, 21},
                       {4, 0, 4, 3, 23}, {4, 4, 4, 3, 23}, {5, 0, 0, -1, 0}, {0, 0, 0, -1, 0}};
    for (const S& s : slots) {
        setup(s.variant, s.kind); norrath_orc_init_type(object, 0);
        // kind 5 sets byte 6 in the first part; those cases are not in this table
        expect(object[156] == s.byte, "final byte", s.variant * 10 + s.kind);
        if (s.owner < 0) expect(calls == 0 && h(1076) == 0xffffu, "no slot work", s.variant * 10 + s.kind);
        else expect(calls == 1 && call_self == owners[s.owner] && h(1076) == s.type && h(1078) == 40u && h(1082) == 0u && rolls == 1, "slot", s.variant * 10 + s.kind);
    }
    std::printf(bad ? "failed\n" : "norrath_orc_init_type passed\n");
    return bad != 0;
}
