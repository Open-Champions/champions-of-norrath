// Host contract for energyCostRamp4__5Skillii. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "energy_cost_ramp4.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
alignas(8) static unsigned char receiver_storage[160];
static unsigned char* receiver = receiver_storage + 16;
alignas(8) static unsigned char table[160];
static struct { unsigned function; unsigned char* self; int first; int second; } calls[4];
static int call_count = 0;

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
    if (call_count < 4) { calls[call_count] = {function, self, first, second}; }
    ++call_count;
    if (function == 0x7000u) return first * 10 + second;   // the tweak slot
    if (function == 0x8000u) return first * 100 + second;  // the evaluate slot
    expect(0, "unknown function word");
    return 0;
}

int main() {
    std::memset(receiver_storage, 0, sizeof receiver_storage);
    std::memset(table, 0, sizeof table);
    *reinterpret_cast<unsigned*>(receiver + 96) = 0x5000u;
    *reinterpret_cast<short*>(table + 120) = -6;
    *reinterpret_cast<unsigned*>(table + 124) = 0x7000u;
    *reinterpret_cast<short*>(table + 88) = 12;
    *reinterpret_cast<unsigned*>(table + 92) = 0x8000u;
    unsigned char before[160];
    std::memcpy(before, receiver_storage, sizeof before);

    const float result = energy_cost_ramp4(receiver, 3, 9);
    expect(call_count == 2, "two calls are made");
    expect(calls[0].function == 0x7000u && calls[0].self == receiver - 6 && calls[0].first == 3 && calls[0].second == 9,
           "first call: slot at +120, adjusted receiver, arguments forwarded");
    // tweak returns 3*10+9 = 39; evaluate gets (4, 39) and returns 4*100+39 = 439
    expect(calls[1].function == 0x8000u && calls[1].self == receiver + 12 && calls[1].first == 4 && calls[1].second == 39,
           "second call: slot at +88, argument 4 and the result of the first call");
    expect(result == 439.0f, "the second result is returned as a float");
    expect(std::memcmp(before, receiver_storage, sizeof before) == 0, "receiver is not written");

    call_count = 0;
    const float negative = energy_cost_ramp4(receiver, -2, 1);
    expect(negative == 4 * 100 + (-2 * 10 + 1) && call_count == 2, "negative arguments flow through");

    if (failures != 0) return 1;
    std::puts("energyCostRamp4 contract passed");
    return 0;
}
