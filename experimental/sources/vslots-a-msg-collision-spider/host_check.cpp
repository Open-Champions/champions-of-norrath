#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#define NORRATH_VSLOTS_A_HOST
#include "collision.cpp"

unsigned long long norrath_vslots_a_host_base;

static std::vector<std::string> calls;
static unsigned char memory[4096];
static int slot_argument = -1;
static void* slot_object = nullptr;
static int failures = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void write_half(unsigned char* p, int value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
static unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer) - norrath_vslots_a_host_base);
}
static void slot_call(void* object, int argument) {
    calls.push_back("slot");
    slot_object = object;
    slot_argument = argument;
}
extern "C" void msg_collision__8CreatureP10GameObject(void* self, void* other) {
    calls.push_back(other == nullptr ? "creature-null" : "creature");
    expect(self == memory, "creature collision receives the spider");
}

int main() {
    norrath_vslots_a_host_base = reinterpret_cast<uintptr_t>(memory);
    unsigned char* spider = memory;
    unsigned char* other = memory + 800;
    unsigned char* vtable = memory + 1600;
    std::memset(memory, 0, sizeof memory);
    write_word(spider + 72, word_of(vtable));
    write_half(vtable + 184, 4);
    write_word(vtable + 188, word_of(reinterpret_cast<void*>(slot_call)));
    write_half(spider + 572, 0);
    write_word(other + 12, 0x40);

    calls.clear();
    msg_collision__11SmallSpiderP10GameObject(spider, other);
    expect(calls.size() == 2 && calls[0] == "slot" && calls[1] == "creature", "slot then creature collision");
    expect(slot_object == spider + 4 && slot_argument == 10, "slot adjusts this and passes 10");

    calls.clear();
    msg_collision__11SmallSpiderP10GameObject(spider, nullptr);
    expect(calls.size() == 1 && calls[0] == "creature-null", "a null other skips the slot");

    calls.clear();
    write_half(spider + 572, 10);
    msg_collision__11SmallSpiderP10GameObject(spider, other);
    expect(calls.size() == 1 && calls[0] == "creature", "state 10 skips the slot");

    calls.clear();
    write_half(spider + 572, 3);
    write_word(other + 12, 0x20);
    msg_collision__11SmallSpiderP10GameObject(spider, other);
    expect(calls.size() == 1 && calls[0] == "creature", "a missing 0x40 bit skips the slot");

    calls.clear();
    write_word(other + 12, 0x41);
    write_half(vtable + 184, static_cast<int>(static_cast<unsigned short>(0xfffe)));
    msg_collision__11SmallSpiderP10GameObject(spider, other);
    expect(calls.size() == 2 && slot_object == spider - 2 && slot_argument == 10, "a negative adjustment is signed");
    return failures ? 1 : 0;
}
