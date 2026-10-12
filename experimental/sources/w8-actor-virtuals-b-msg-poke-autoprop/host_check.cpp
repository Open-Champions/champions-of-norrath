#include <cstdio>
#include <cstring>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
static void advance_stub(void* self);
extern "C" void* norrath_w8avb_host_function(unsigned word) {
    return word == 7 ? reinterpret_cast<void*>(&advance_stub) : nullptr;
}
#include "msg_poke.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char memory[1024];
static int calls;
static void* seen;
static void advance_stub(void* self) {
    ++calls;
    seen = self;
}
static void put(unsigned off, unsigned value) { std::memcpy(memory + off, &value, 4); }

static void setup(unsigned flags, unsigned state) {
    std::memset(memory, 0, sizeof memory);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(memory);
    // object at 0, table at 512; entry at table+136 holds adjustment 24 and function word 7
    put(72, 512);
    put(284, flags);
    put(288, state);
    short adjust = 24;
    std::memcpy(memory + 512 + 136, &adjust, 2);
    put(512 + 136 + 4, 7);
    put(512 + 144 + 4, 9);  // the neighbouring entry must not be used
    calls = 0;
    seen = nullptr;
}

int main() {
    setup(0x10, 1);
    msg_poke__8AutoPropi(memory, 99);
    expect(calls == 1, "calls the slot once");
    expect(seen == memory + 24, "object address is adjusted");
    setup(0x10, 0);
    msg_poke__8AutoPropi(memory, 99);
    expect(calls == 0, "zero state: no call");
    setup(0x00, 5);
    msg_poke__8AutoPropi(memory, 99);
    expect(calls == 0, "bit 4 clear: no call");
    setup(0xffffffefu, 5);
    msg_poke__8AutoPropi(memory, 99);
    expect(calls == 0, "every other flag bit does not matter");
    setup(0x11, 0x80000000u);
    msg_poke__8AutoPropi(memory, 0);
    expect(calls == 1, "any nonzero state calls");
    return failures ? 1 : 0;
}
