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
static void setup(unsigned variant, unsigned kind, unsigned mask = 0) {
    std::memset(object, 0x5a, sizeof object);
    rtti_c::write_word(a() + 208u, variant); object[960] = static_cast<unsigned char>(kind); rtti_c::write_word(a() + 244u, mask);
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
    setup(0, 0); norrath_male_dark_elf_init_type(object, 3);
    expect(params_calls == 1 && params_flag == 2 && params_bits[0] == 0x3fb33333u && params_bits[1] == 0x3f999999u && params_bits[2] == 0x3f800000u && params_bits[3] == 0x3f800000u && params_bits[4] == 0x3f800000u, "parameters");
    setup(0, 0); norrath_male_dark_elf_init_type(object, 0); expect(params_flag == 1, "flag 0 gives 1");
    expect(h(1076) == 0xffffu && h(1084) == 0xffffu && h(1092) == 0xffffu && h(1100) == 0xffffu, "slots cleared");
    setup(0, 5); norrath_male_dark_elf_init_type(object, 0); expect(w(96) == 0x3e4cccccu, "kind 5 stores +96");
    setup(0, 4); norrath_male_dark_elf_init_type(object, 0); expect(w(96) == 0x5a5a5a5au, "kind 4 leaves +96");
    setup(0, 0xff); norrath_male_dark_elf_init_type(object, 0); expect(w(96) == 0x5a5a5a5au && object[156] == 0, "negative kind");
    struct S { unsigned variant, kind, byte; int owner; unsigned type; };
    const S cases[] = {
        {1, 0, 3, -1, 0}, {1, 1, 4, 3, 23}, {1, 0x80, 4, 3, 23},
        {2, 0, 2, -1, 0}, {2, 3, 8, -1, 0}, {3, 0, 2, -1, 0}, {3, 2, 8, -1, 0},
        {4, 0, 4, -1, 0}, {4, 1, 3, 2, 22}, {4, 2, 1, 0, 24}, {4, 3, 2, 1, 21}, {4, 4, 0, -1, 0},
        {5, 0, 8, -1, 0}, {5, 1, 0, -1, 0}, {0, 0, 0, -1, 0}, {6, 0, 0, -1, 0}};
    for (const S& s : cases) {
        setup(s.variant, s.kind); norrath_male_dark_elf_init_type(object, 0);
        expect(object[156] == s.byte, "final byte", s.variant * 10 + s.kind);
        if (s.owner < 0) expect(calls == 0 && h(1076) == 0xffffu, "no slot work", s.variant * 10 + s.kind);
        else expect(calls == 1 && call_self == owners[s.owner] && h(1076) == s.type && h(1078) == 40u && h(1082) == 0u && rolls == 1, "slot", s.variant * 10 + s.kind);
    }
    setup(3, 1); norrath_male_dark_elf_init_type(object, 0); expect(w(212) == 1u, "variant 3 sets +212");
    setup(3, 0); norrath_male_dark_elf_init_type(object, 0); expect(w(212) == 0x5a5a5a5au, "variant 3 kind 0 leaves +212");
    const unsigned long long keep = 0xffffffc9ffffffffull;
    struct M { unsigned mask; unsigned bit; };
    const M masks[] = {{0x1, 36}, {0x4112e6b, 36}, {0x400000, 33}, {0x2000, 36}, {0xd100, 37}, {0x100, 37}, {0x14, 34}, {0x1a00000, 34}, {0x4, 34}, {0x10000000, 33}, {0, 33}, {0x1000000, 34}};
    for (const M& m : masks) {
        setup(0, 0, m.mask); norrath_male_dark_elf_init_type(object, 0);
        expect(d(160) == (keep | (1ull << m.bit)), "flag bits", static_cast<int>(m.mask));
    }
    std::printf(bad ? "failed\n" : "norrath_male_dark_elf_init_type passed\n");
    return bad != 0;
}
