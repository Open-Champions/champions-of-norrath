// Host contract for evaluateRampAtRank__5SkilliiP10GameObject. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "evaluate_ramp_at_rank_object.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
alignas(8) static unsigned char receiver_storage[160];
static unsigned char* receiver = receiver_storage + 16;
alignas(8) static unsigned char table[160];
alignas(8) static unsigned char object_block[32];
alignas(8) static unsigned char node_block[32];
alignas(8) static unsigned char head_block[16];
static struct { unsigned function; unsigned char* self; int first; int second; } last;
static int call_count = 0;

static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}

extern "C" unsigned char* norrath_host_word_pointer(unsigned word) {
    if (word == 0x5000u) return table;
    if (word == 0x6000u) return node_block;
    if (word == 0x7000u) return object_block;
    std::fprintf(stderr, "unexpected pointer word %u\n", word);
    ++failures;
    return table;
}

extern "C" int norrath_host_call_slot(unsigned function, unsigned char* self, int first, int second) {
    last = {function, self, first, second};
    ++call_count;
    return function == 0x8000u ? 1000 + first + second : 2000 + first + second;
}

static void reset() {
    std::memset(receiver_storage, 0, sizeof receiver_storage);
    std::memset(table, 0, sizeof table);
    std::memset(object_block, 0, sizeof object_block);
    std::memset(node_block, 0, sizeof node_block);
    std::memset(head_block, 0, sizeof head_block);
    *reinterpret_cast<unsigned*>(receiver + 96) = 0x5000u;
    *reinterpret_cast<short*>(table + 88) = 12;
    *reinterpret_cast<unsigned*>(table + 92) = 0x8000u;
    *reinterpret_cast<short*>(table + 96) = -6;
    *reinterpret_cast<unsigned*>(table + 100) = 0x9000u;
    call_count = 0;
}

int main() {
    reset();
    int result = evaluate_ramp_at_rank_object(receiver, 3, 9, nullptr);
    expect(call_count == 1 && last.function == 0x9000u && last.self == receiver - 6 && last.first == 3 && last.second == 9,
           "null object: slot at +96 with adjustment and forwarded arguments");
    expect(result == 2012, "null object: the slot result is returned");

    reset();
    *reinterpret_cast<unsigned*>(object_block + 12) = 0x40u;
    result = evaluate_ramp_at_rank_object(receiver, 4, 5, object_block);
    expect(call_count == 1 && last.function == 0x8000u && last.self == receiver + 12 && last.first == 4 && last.second == 5,
           "bit 0x40 set: slot at +88 with adjustment and forwarded arguments");
    expect(result == 1009, "bit 0x40 set: the slot result is returned");

    reset();
    *reinterpret_cast<unsigned*>(object_block + 12) = 0xffffffbfu;
    result = evaluate_ramp_at_rank_object(receiver, -1, 2, object_block);
    expect(call_count == 1 && last.function == 0x9000u && last.first == -1 && last.second == 2,
           "every other bit set but 0x40 clear: slot at +96");
    expect(result == 2001, "bit clear: the slot result is returned");

    reset();
    *reinterpret_cast<unsigned*>(object_block + 12) = 0x80u;
    (void)evaluate_ramp_at_rank_object(receiver, 0, 0, object_block);
    expect(last.function == 0x9000u, "0x80 alone is not the tested bit");
    if (failures != 0) return 1;
    std::puts("evaluateRampAtRank__5SkilliiP10GameObject contract passed");
    return 0;
}
