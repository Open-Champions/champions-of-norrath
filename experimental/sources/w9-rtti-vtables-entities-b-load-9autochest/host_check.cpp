#include <cstdio>
#include <cstring>
#include <vector>

#include "load.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::vector<int> order;
static void* add_state;
static void* add_header;
static int add_flags;
static void* end_state;
static unsigned char* current_object;
static unsigned flags_at_add;
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags) {
    order.push_back(1);
    add_state = state;
    add_header = header;
    add_flags = flags;
    std::memcpy(&flags_at_add, current_object + 12, 4);
}
extern "C" void animSetToEnd__FP14AnimationState(void* state) {
    order.push_back(2);
    end_state = state;
}

int main() {
    alignas(8) unsigned char object[512];
    struct Case { int first; short half; signed char a, b; unsigned last; };
    const Case cases[] = {{0, 0, 0, 0, 0},         {1, 5, 1, 2, 0},          {-1, -5, -1, -2, 0},
                          {0x7fffffff, 1, 3, 4, 1}, {static_cast<int>(0x80000000u), 0, 0x7f, -128, 9},
                          {42, 100, 5, 6, 0xffffffffu}, {-77, -1, 7, 8, 3}, {5, 32767, 1, 1, 2}, {5, -32768, 1, 1, 2}};
    for (const Case& c : cases) {
        std::memset(object, 0x33, sizeof object);
        unsigned all = 0xffffffffu;
        std::memcpy(object + 12, &all, 4);
        const unsigned header_word = 0x5000;
        std::memcpy(object + 244, &header_word, 4);
        current_object = object;
        alignas(4) char stream[16];
        std::memset(stream, 0x66, sizeof stream);
        std::memcpy(stream, &c.first, 4);
        std::memcpy(stream + 4, &c.half, 2);
        stream[6] = c.a;
        stream[7] = c.b;
        std::memcpy(stream + 8, &c.last, 4);
        order.clear();
        int used = msg_load__9AutoChestPc(object, stream);
        expect(used == 12, "consumes twelve bytes");
        long long quad;
        short half;
        unsigned last;
        std::memcpy(&quad, object + 200, 8);
        std::memcpy(&half, object + 220, 2);
        std::memcpy(&last, object + 216, 4);
        expect(quad == static_cast<long long>(c.first), "word stored sign-extended to 64 bits at +200");
        expect(half == c.half && object[228] == static_cast<unsigned char>(c.a) && object[229] == static_cast<unsigned char>(c.b) && last == c.last, "other fields");
        expect(object[199] == 0x33 && object[208] == 0x33 && object[222] == 0x33 && object[230] == 0x33, "neighbours untouched");
        unsigned flags;
        std::memcpy(&flags, object + 12, 4);
        if (c.last != 0) {
            expect(order.size() == 2 && order[0] == 1 && order[1] == 2, "animAdd then animSetToEnd");
            expect(add_state == object + 152 && end_state == object + 152, "animation state at +152");
            expect(add_header == reinterpret_cast<void*>(0x5000) && add_flags == 516, "header word at +244, flags 516");
            expect(flags_at_add == all, "flag word cleared only after the calls");
            expect(flags == (c.half > 0 ? all : 0xffefffffu), "bit 20 cleared unless the halfword is positive");
        } else {
            expect(order.empty() && flags == all, "last word zero: no calls");
        }
    }
    return failures ? 1 : 0;
}
