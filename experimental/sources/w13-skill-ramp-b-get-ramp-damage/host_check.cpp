// Host contract for getRampDamage__5SkilliifiPiT5. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "get_ramp_damage.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

unsigned syncRandState = 0;
static int failures = 0;
alignas(8) static unsigned char receiver_storage[160];
static unsigned char* receiver = receiver_storage + 16;
alignas(8) static unsigned char table[160];
static unsigned char* last_self;
static int call_count = 0;
static int last_first, last_second;

static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}

extern "C" unsigned char* norrath_host_word_pointer(unsigned word) {
    if (word == 0x5000u) return table;
    std::fprintf(stderr, "unexpected pointer word %u\n", word);
    ++failures;
    return table;
}

extern "C" int norrath_host_call_slot(unsigned function, unsigned char* self, int first, int second) {
    expect(function == 0x8000u, "only the +88 slot is called");
    last_self = self; last_first = first; last_second = second;
    ++call_count;
    return second;  // the level is the ramp value
}

static int reference_round(float x) { return x >= 0.0f ? static_cast<int>(x + 0.5f) : static_cast<int>(x - 0.5f); }

int main() {
    std::memset(receiver_storage, 0, sizeof receiver_storage);
    std::memset(table, 0, sizeof table);
    *reinterpret_cast<unsigned*>(receiver + 96) = 0x5000u;
    *reinterpret_cast<short*>(table + 88) = 12;
    *reinterpret_cast<unsigned*>(table + 92) = 0x8000u;

    int low = -99, high = -99;
    syncRandState = 0x1234u;
    get_ramp_damage(receiver, 3, 100, 0.25f, 0, &low, &high);
    expect(call_count == 1 && last_self == receiver + 12 && last_first == 3 && last_second == 100, "slot call arguments");
    expect(low == 75 && high == 125, "100 with spread 0.25 gives 75..125");
    expect(syncRandState == 0x1234u, "no draw when the randomize flag is zero");

    get_ramp_damage(receiver, 0, 3, 0.5f, 0, &low, &high);
    expect(low == 2 && high == 5, "1.5 and 4.5 round away from zero");
    get_ramp_damage(receiver, 0, 1, 2.0f, 0, &low, &high);
    expect(low == 1 && high == 3, "a negative low is replaced by 1");
    get_ramp_damage(receiver, 0, 0, 0.5f, 0, &low, &high);
    expect(low == 1 && high == 1, "zero base gives 1 and 1");
    get_ramp_damage(receiver, 0, -10, 0.5f, 0, &low, &high);
    expect(low == 1 && high == 1, "negative base is clamped to 1 on both sides");

    // randomized: four steps of the generator, two per output, low first.
    unsigned state = 0xdeadbeefu;
    syncRandState = state;
    float draws[4];
    for (int i = 0; i < 4; ++i) {
        state = state * 0x19660du + 0x3c6ef35fu;
        draws[i] = static_cast<float>(static_cast<int>(state >> 16)) * 1.52587890625e-05f;
    }
    const float v = 1000.0f, spread = 0.5f;
    const int expect_low = reference_round(v - v * draws[1] * spread);
    const int expect_high = reference_round(v + v * draws[3] * spread);
    get_ramp_damage(receiver, 1, 1000, spread, 1, &low, &high);
    expect(syncRandState == state, "four generator steps");
    expect(low == expect_low && high == expect_high, "second draw of each pair feeds the rounded value");
    expect(low >= 500 && low <= 1000 && high >= 1000 && high <= 1500, "range stays inside the spread");

    // the first draw of each pair only decides the sign branch: a different first draw cannot change the result
    // when the value is positive in both cases, so check the draw count with a state that makes it observable.
    syncRandState = 0;
    get_ramp_damage(receiver, 0, 10, 0.0f, 7, &low, &high);
    expect(low == 10 && high == 10 && syncRandState != 0u, "any nonzero flag randomizes; zero spread is exact");

    if (failures != 0) return 1;
    std::puts("getRampDamage contract passed");
    return 0;
}
