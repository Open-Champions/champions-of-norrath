// Host check only; not an EE build input. Build and run:
//   c++ -std=c++17 -Wall -Wextra -Werror -DNORRATH_HOST_ARENA host_check.cpp && ./a.out
// The object and table words are 32-bit arena offsets; offset 0xF00D stands for the ramp function.
#include <cstdio>
#include <cstring>
#include "get_rank_description.cpp"

static unsigned char arena[0x1000];
static int ramp_calls, ramp_ramp, ramp_rank;
static unsigned char* ramp_object;
static int ramp_result;
static int rampStub(unsigned char* object, int ramp, int rank) {
    ++ramp_calls; ramp_object = object; ramp_ramp = ramp; ramp_rank = rank;
    return ramp_result;
}
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    if (address == 0xF00D) return reinterpret_cast<unsigned char*>(rampStub);
    return arena + address;
}
static const char* text_key;
static unsigned short format_text[4];
static int calls_text, calls_us;
static unsigned short* us_dst;
static unsigned short* us_fmt;
static float us_arg[8];
extern "C" unsigned short* textGetString__FPc(const char* key) {
    text_key = key; ++calls_text;
    return format_text;
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c, float e,
                                         float g, float h, float i, float j) {
    ++calls_us; us_dst = d; us_fmt = f;
    float v[8] = {a, b, c, e, g, h, i, j};
    std::memcpy(us_arg, v, sizeof v);
}
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }

int main() {
    const int ranks[] = {-5, 0, 1, 7, 100};
    const int results[] = {-70000, -1, 0, 3, 16777217, 2000000000};
    const short deltas[] = {0, 24, -8};
    int bad = 0;
    for (short delta : deltas)
        for (int rank : ranks)
            for (int result : results) {
                std::memset(arena, 0, sizeof arena);
                put32(0x100 + 96, 0x400);                 // object -> table
                std::memcpy(arena + 0x400 + 88, &delta, 2);  // entry at +88
                put32(0x400 + 92, 0xF00D);
                ramp_calls = calls_text = calls_us = 0; ramp_result = result;
                getRankDescription__10SkillDodgePUsi(arena + 0x100, reinterpret_cast<unsigned short*>(arena + 0x800), rank);
                bool ok = ramp_calls == 1 && ramp_ramp == 0 && ramp_rank == rank && ramp_object == arena + 0x100 + delta &&
                          calls_text == 1 && calls_us == 1 && std::strcmp(text_key, "dodge_rankdesc") == 0 &&
                          us_dst == reinterpret_cast<unsigned short*>(arena + 0x800) && us_fmt == format_text &&
                          us_arg[0] == static_cast<float>(result);
                for (int k = 1; k < 8; ++k) ok = ok && us_arg[k] == 0.0f;
                if (!ok) { std::printf("fail delta=%d rank=%d result=%d\n", delta, rank, result); bad = 1; }
            }
    return bad;
}
