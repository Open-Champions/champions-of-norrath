// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "init_type.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
unsigned char skills[252];
static int roll_value = 0, rolls = 0, call_result = 0, calls = 0, params_flag = -9, params_calls = 0;
static unsigned params_bits[5];
static void* call_self = nullptr;
static unsigned char object[1300];
static unsigned char owners[3][128], tables[3][128];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" int eRand__10GameObject(void*) { ++rolls; return roll_value; }
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
static void setup(unsigned variant, unsigned selector, unsigned mask, unsigned byte972) {
    std::memset(object, 0x5a, sizeof object);
    rtti_c::write_word(a() + 208u, variant); rtti_c::write_word(a() + 968u, selector); rtti_c::write_word(a() + 244u, mask);
    object[972] = static_cast<unsigned char>(byte972);
    rtti_c::write_u64(a() + 160u, 0xffffffffffffffffull);
    for (int k = 0; k < 3; ++k) {
        std::memset(owners[k], 0, sizeof owners[k]); std::memset(tables[k], 0, sizeof tables[k]);
        rtti_c::host_note(owners[k]); rtti_c::host_note(tables[k]);
        rtti_c::write_word(rtti_c::address(owners[k]) + 96u, rtti_c::address_bits(tables[k]));
        rtti_c::write_word(rtti_c::address(tables[k]) + 100u, rtti_c::address_bits(reinterpret_cast<void*>(vslot)));
    }
    rtti_c::host_note(reinterpret_cast<void*>(vslot));
    rtti_c::write_word(rtti_c::address(skills) + 48u, rtti_c::address_bits(owners[2]));
    rtti_c::write_word(rtti_c::address(skills) + 52u, rtti_c::address_bits(owners[0]));
    rtti_c::write_word(rtti_c::address(skills) + 56u, rtti_c::address_bits(owners[1]));
    calls = 0; rolls = 0; params_calls = 0; call_result = 40; roll_value = 0; call_self = nullptr;
}
int main() {
    const unsigned long long keep = 0xfffffed7ffffffffull;
    // flag word and parameters
    setup(0, 0, 0, 0); norrath_skeleton_init_type(object, 1);
    expect(params_calls == 1 && params_flag == 0 && params_bits[0] == 0x3fa66666u && params_bits[1] == 0x3f999999u && params_bits[2] == 0x3f800000u && params_bits[3] == 0x3f800000u && params_bits[4] == 0x3f800000u, "parameters, flag 1");
    expect(d(160) == (keep | (1ull << 40)) && object[156] == 0, "no mask: bit 40, byte cleared");
    expect(w(788) == 0x447a0000u, "1000.0 when +972 is zero");
    expect(h(1084) == 0xffffu && h(1092) == 0xffffu && h(1100) == 0xffffu && h(1108) == 0xffffu, "slots cleared");
    setup(0, 0, 0, 0); norrath_skeleton_init_type(object, 0); expect(params_flag == 1, "flag 0 -> 1");
    setup(0, 0, 0, 0); norrath_skeleton_init_type(object, 5); expect(params_flag == 4, "flag 5 -> 4");
    for (unsigned mask : {0x200u, 0x400u, 0x800u, 0x1000u, 0x20000u}) { setup(0, 0, mask, 1); norrath_skeleton_init_type(object, 0); expect(d(160) == (keep | (1ull << 35)) && w(788) == 0x43480000u, "bit 35 masks", static_cast<int>(mask)); }
    for (unsigned mask : {0x2000u, 0x4000u, 0x8000u, 0x10000u, 0x40000u, 0x80000u, 0x100000u}) { setup(0, 0, mask, 0x80); norrath_skeleton_init_type(object, 0); expect(d(160) == (keep | (1ull << 37)), "bit 37 masks", static_cast<int>(mask)); }
    setup(0, 0, 0x200000u, 0); norrath_skeleton_init_type(object, 0); expect(d(160) == (keep | (1ull << 40)), "mask outside both sets");
    // variant tree
    struct Case { unsigned variant, selector, byte; int row; unsigned type; };
    const Case cases[] = {
        {1, 4, 3, -1, 0}, {1, 2, 2, 13, 21}, {1, 3, 2, 13, 21}, {1, 0, 1, 12, 24}, {1, 1, 1, 12, 24}, {1, 5, 1, 12, 24},
        {2, 4, 2, -1, 0}, {2, 0, 3, 14, 22}, {2, 3, 3, 14, 22},
        {3, 4, 2, -1, 0}, {3, 0, 0, -1, 0}, {4, 4, 8, -1, 0}, {4, 2, 0, -1, 0}, {5, 4, 0, -1, 0}, {0, 4, 0, -1, 0}};
    for (const Case& c : cases) {
        setup(c.variant, c.selector, 0, 0);
        norrath_skeleton_init_type(object, 0);
        expect(object[156] == c.byte, "final byte", c.variant * 10 + c.selector);
        if (c.row < 0) {
            expect(calls == 0 && rolls == 0 && h(1084) == 0xffffu, "no slot work", c.variant * 10 + c.selector);
        } else {
            const int owner = c.row == 13 ? 0 : c.row == 14 ? 1 : 2;
            expect(calls == 1 && call_self == owners[owner] && rolls == 1, "slot call", c.variant * 10 + c.selector);
            expect(h(1084) == c.type && h(1086) == 40u && h(1090) == 0u && h(1092) == 0xffffu, "slot 0", c.variant * 10 + c.selector);
        }
    }
    std::printf(bad ? "failed\n" : "norrath_skeleton_init_type passed\n");
    return bad != 0;
}
