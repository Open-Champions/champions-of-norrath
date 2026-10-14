// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "init_weapon.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
unsigned char skills[252];
static int roll_value = 0, rolls = 0, call_result = 0, call_arg1 = -1, call_arg2 = -1, calls = 0;
static void* call_self = nullptr;
static bool mutate = false;
static unsigned mutated[4];
static unsigned char object[1300];
extern "C" int eRand__10GameObject(void*) { ++rolls; return roll_value; }
static int vslot(void* self, int a, int b) {
    ++calls; call_self = self; call_arg1 = a; call_arg2 = b;
    if (mutate) for (unsigned i = 0; i < 4; ++i) rtti_c::write_u16(rtti_c::address(object) + 1068u + 8u * i, mutated[i]);
    return call_result;
}
static unsigned char owners[3][128];
static unsigned char tables[3][128];
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned h(unsigned off) { return rtti_c::read_u16(a() + off); }
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0) { if (!ok) { std::printf("fail: %s (%d)\n", msg, x); ++bad; } }
static void setup(unsigned variant, int adjustment) {
    std::memset(object, 0x5a, sizeof object);
    rtti_c::write_word(a() + 208u, variant);
    for (int k = 0; k < 3; ++k) {
        std::memset(owners[k], 0, sizeof owners[k]); std::memset(tables[k], 0, sizeof tables[k]);
        rtti_c::host_note(owners[k]); rtti_c::host_note(tables[k]);
        rtti_c::write_word(rtti_c::address(owners[k]) + 96u, rtti_c::address_bits(tables[k]));
        rtti_c::write_u16(rtti_c::address(tables[k]) + 96u, static_cast<unsigned>(adjustment));
        rtti_c::write_word(rtti_c::address(tables[k]) + 100u, rtti_c::address_bits(reinterpret_cast<void*>(vslot)));
    }
    rtti_c::host_note(reinterpret_cast<void*>(vslot));
    // skills[12] -> owner 2, skills[13] -> owner 0, skills[14] -> owner 1
    rtti_c::write_word(rtti_c::address(skills) + 48u, rtti_c::address_bits(owners[2]));
    rtti_c::write_word(rtti_c::address(skills) + 52u, rtti_c::address_bits(owners[0]));
    rtti_c::write_word(rtti_c::address(skills) + 56u, rtti_c::address_bits(owners[1]));
    calls = 0; rolls = 0; call_self = nullptr;
}
int main() {
    // rows: variant -> (owner, type, final byte)
    const struct { unsigned variant, owner, type, fin; } rows[3] = {{1, 0, 21, 2}, {2, 1, 22, 3}, {3, 2, 24, 1}};
    for (const auto& row : rows) {
        setup(row.variant, 16);
        call_result = 50; roll_value = 65536;  // 50 * 0.19999999 = 9.9999995, times 1.0, truncated: +9
        norrath_skelight_init_weapon(object);
        expect(calls == 1 && call_self == owners[row.owner] + 16 && call_arg1 == 0 && call_arg2 == 1, "virtual call", row.variant);
        expect(rolls == 1, "one roll", row.variant);
        expect(h(1068) == row.type && h(1070) == 50u && h(1072) == 59u && h(1074) == 0u, "slot 0 filled", row.variant);
        expect(h(1076) == 0xffffu && h(1084) == 0xffffu && h(1092) == 0xffffu, "other slots cleared", row.variant);
        expect(object[156] == row.fin, "final byte", row.variant);
    }
    // negative adjustment, negative result and a fractional roll
    setup(2, -8); call_result = -30; roll_value = 32768;  // -30 * 0.19999999 = -5.9999995, * 0.5 = -2.99999975, truncated: -2
    norrath_skelight_init_weapon(object);
    expect(call_self == owners[1] - 8, "negative adjustment");
    expect(h(1070) == 0xffe2u && h(1072) == 0xffe0u, "negative amounts");
    // unmatched variants
    for (unsigned v : {0u, 4u, 0xffffffffu, 100u}) {
        setup(v, 0); norrath_skelight_init_weapon(object);
        expect(calls == 0 && rolls == 0 && object[156] == 0 && h(1068) == 0xffffu && h(1092) == 0xffffu, "unmatched variant", static_cast<int>(v));
    }
    // slot choice when the callee changed the slot types
    mutate = true;
    const unsigned cases[][4] = {{5, 6, 0xffff, 7}, {5, 22, 6, 22}, {5, 6, 7, 8}, {1, 2, 3, 22}};
    const int where[] = {2, 1, -1, 3};
    for (int c = 0; c < 4; ++c) {
        setup(2, 0); call_result = 10; roll_value = 0;
        for (int i = 0; i < 4; ++i) mutated[i] = cases[c][i];
        norrath_skelight_init_weapon(object);
        for (int i = 0; i < 4; ++i) {
            const bool target = i == where[c];
            if (target) expect(h(1068 + 8 * i) == 22u && h(1070 + 8 * i) == 10u && h(1072 + 8 * i) == 10u && h(1074 + 8 * i) == 0u, "chosen slot", c);
            else expect(h(1068 + 8 * i) == cases[c][i], "other slots untouched", c);
        }
        expect(object[156] == 3, "final byte when no slot qualifies", c);
    }
    mutate = false;
    std::printf(bad ? "failed\n" : "norrath_skelight_init_weapon passed\n");
    return bad != 0;
}
