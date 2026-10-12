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
static int ready_result = 1;
static void* slot_object = nullptr;
static int slot_argument = -1;
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
extern "C" int readyToSwitch__3Cat(void* self) {
    calls.push_back(self == memory ? "ready" : "ready-other");
    return ready_result;
}
extern "C" void switchToRun__3Cat(void* self) {
    calls.push_back(self == memory ? "run" : "run-other");
}
static void slot_call(void* object, int argument) {
    calls.push_back("slot");
    slot_object = object;
    slot_argument = argument;
}

int main() {
    norrath_vslots_a_host_base = reinterpret_cast<uintptr_t>(memory);
    unsigned char* cat = memory;
    unsigned char* other = memory + 2000;
    unsigned char* vtable = memory + 2200;
    std::memset(memory, 0x2b, sizeof memory);
    write_word(cat + 72, word_of(vtable));
    write_half(vtable + 184, 8);
    write_word(vtable + 188, word_of(reinterpret_cast<void*>(slot_call)));
    write_word(cat + 964, 0);
    write_half(cat + 572, 0);
    ready_result = 1;

    calls.clear();
    msg_collision__3CatP10GameObject(cat, other);
    expect(calls.size() == 3 && calls[0] == "ready" && calls[1] == "run" && calls[2] == "slot", "state 0 runs the slot");
    expect(slot_object == cat + 8 && slot_argument == 3, "slot adjusts this and passes 3");
    expect(other[0] == 0x2b, "the other object is not written");

    calls.clear();
    write_word(cat + 964, 1);
    msg_collision__3CatP10GameObject(cat, other);
    expect(calls.empty(), "word 1 at cat+964 returns before the helpers");

    calls.clear();
    write_word(cat + 964, 2);
    write_half(cat + 572, 13);
    msg_collision__3CatP10GameObject(cat, other);
    expect(calls.size() == 3 && calls[0] == "ready", "state 13 also runs the slot");

    calls.clear();
    write_half(cat + 572, 4);
    msg_collision__3CatP10GameObject(cat, other);
    expect(calls.empty(), "any other state returns");

    calls.clear();
    write_half(cat + 572, 0);
    ready_result = 0;
    msg_collision__3CatP10GameObject(cat, other);
    expect(calls.size() == 1 && calls[0] == "ready", "a zero ready result skips the switch");
    return failures ? 1 : 0;
}
