// Host contract for set__11LavaTractor. Not an EE plan input.
// table.inc lists every (offset, word) the original stores, in order. It is
// numbers observed in the pinned executable, not code.
#include <cstdio>
#include <cstring>
extern "C" void set__11LavaTractor(void* object);

struct Entry { unsigned offset; unsigned bits; };
static const Entry table[] = {
#include "table.inc"
};
static const unsigned count = sizeof table / sizeof table[0];
// How many table entries must already be stored when each helper call happens.
static const unsigned before_call[] = {4, 24, 24, 24, 24};
static const unsigned pointer_offsets[4][2] = {{2056, 2328}, {2060, 2344}, {2064, 2344}, {2068, 2328}};
static unsigned char object[2400];
static unsigned present[5];
static int calls_seen = 0, mass_calls = 0, wheel_count = 0;
static char* wheel_model[4]; static int wheel_flag[4]; static float wheel_args[4][4];
static unsigned word(const unsigned char* base, unsigned off) { unsigned v; std::memcpy(&v, base + off, 4); return v; }
// Record how many entries are present at call time, and that none later are.
static void snapshot(int slot, void* self) {
    unsigned n = 0, later = 0;
    for (unsigned i = 0; i < count; ++i) {
        const bool stored = word(object, table[i].offset) == table[i].bits;
        if (stored && i == n) ++n; else if (stored) ++later;
    }
    present[slot] = n | (later << 16);
    if (self != object) std::fprintf(stderr, "bad object argument\n");
    ++calls_seen;
}
extern "C" void setMassAndIBody__7Vehicle(void* self) { ++mass_calls; snapshot(0, self); }
extern "C" unsigned addWheel__7VehiclePciffff(void* object, char* model, int flag, float a, float b, float c, float d) {
    if (wheel_count < 4) {
        wheel_model[wheel_count] = model; wheel_flag[wheel_count] = flag;
        wheel_args[wheel_count][0] = a; wheel_args[wheel_count][1] = b; wheel_args[wheel_count][2] = c; wheel_args[wheel_count][3] = d;
        snapshot(1 + wheel_count, object);
    }
    ++wheel_count;
    return 0;
}
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n", m); return 1; } } while (0)
int main() {
    std::memset(object, 0xa5, sizeof object);
    set__11LavaTractor(object);
    EXPECT(mass_calls == 1 && calls_seen == 5, "helper call count");
    for (int i = 0; i < 5; ++i) EXPECT(present[i] == before_call[i], "stores around helper call");
    for (unsigned i = 0; i < count; ++i) EXPECT(word(object, table[i].offset) == table[i].bits, "stored word");
    for (unsigned i = 0; i < 4; ++i) {
        const unsigned want = static_cast<unsigned>(reinterpret_cast<unsigned long>(object + pointer_offsets[i][1]));
        EXPECT(word(object, pointer_offsets[i][0]) == want, "pointer word");
    }
    // nothing else was written
    unsigned char expected[2400];
    std::memset(expected, 0xa5, sizeof expected);
    for (unsigned i = 0; i < count; ++i) std::memcpy(expected + table[i].offset, &table[i].bits, 4);
    for (unsigned i = 0; i < 4; ++i) { const unsigned w = word(object, pointer_offsets[i][0]); std::memcpy(expected + pointer_offsets[i][0], &w, 4); }
    EXPECT(std::memcmp(expected, object, sizeof expected) == 0, "no stray stores");
    EXPECT(wheel_count == 4, "four wheels");
    static const char* const models[4] = {"wheelfrnt.vif", "wheelbk.vif", "wheelbk.vif", "wheelfrnt.vif"};
    static const float args[4][4] = {{1.0f, 31.0f, -89.0f, -19.0f}, {0.0f, 47.0f, 63.0f, -19.0f}, {0.0f, -47.0f, 63.0f, -19.0f}, {1.0f, -31.0f, -89.0f, -19.0f}};
    for (int i = 0; i < 4; ++i) {
        EXPECT(wheel_model[i] != nullptr && std::strcmp(wheel_model[i], models[i]) == 0, "wheel model");
        EXPECT(wheel_flag[i] == 1, "wheel flag");
        for (int j = 0; j < 4; ++j) EXPECT(wheel_args[i][j] == args[i][j], "wheel float");
    }
    EXPECT(wheel_model[0] == wheel_model[3] && wheel_model[1] == wheel_model[2], "model strings shared");
    return 0;
}
