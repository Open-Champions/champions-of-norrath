#include <cstdio>
#include <cstring>
#include <initializer_list>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
extern "C" void* norrath_w8avb_host_function(unsigned) { return nullptr; }
extern "C" unsigned frameNm = 0;

static int base_calls;
static void* base_self;
static int base_value;
alignas(8) static unsigned char memory[1024];
static unsigned seen_at_call[4];
extern "C" void msg_poke__9AutoChesti(void* object, int value) {
    ++base_calls;
    base_self = object;
    base_value = value;
    std::memcpy(&seen_at_call[0], memory + 512 + 92, 4);
}

#include "msg_poke.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void put(unsigned off, unsigned value) { std::memcpy(memory + off, &value, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, memory + off, 4); return v; }

int main() {
    for (unsigned gate : {0u, 1u, 0x80000000u}) {
        for (unsigned other : {0u, 512u}) {
            std::memset(memory, 0x5a, sizeof memory);
            norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(memory);
            put(216, gate);
            put(248, other);
            put(512 + 144, 1000);
            frameNm = 77;
            base_calls = 0;
            msg_poke__11AutoChestLPi(memory, 31337);
            expect(base_calls == 1 && base_self == memory && base_value == 31337, "always calls msg_poke__9AutoChesti with the arguments");
            if (gate == 0 && other != 0) {
                expect(memory[512 + 160] == 1, "byte +160 is 1");
                expect(get(512 + 92) == 1 && get(512 + 96) == 1, "words +92 and +96 are 1");
                expect(get(512 + 156) == 1077, "word +156 is frameNm plus the word at +144");
                expect(seen_at_call[0] == 1, "writes happen before the call");
                expect(memory[512 + 161] == 0x5a && get(512 + 100) == 0x5a5a5a5au, "neighbours untouched");
            } else {
                expect(get(512 + 92) == 0x5a5a5a5au && memory[512 + 160] == 0x5a, "no writes");
            }
        }
    }
    return failures ? 1 : 0;
}
