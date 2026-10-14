#include <cstdio>
#include <cstring>

#include "msg_poke.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char object[160];
static int added;
static int state_at_add;
static void* added_object;
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    ++added;
    added_object = o;
    state_at_add = *reinterpret_cast<int*>(object + 128);
}
static int& word(unsigned off) { return *reinterpret_cast<int*>(object + off); }

static void fresh(int options, int state) {
    std::memset(object, 0x11, sizeof object);
    word(12) = static_cast<int>(0xffffffffu);
    word(104) = options;
    word(128) = state;
    word(116) = 99;
    added = 0;
}

int main() {
    fresh(4, 0);
    msg_poke__9Containeri(object, 12345);
    expect(added == 1 && added_object == object, "added to the run list once");
    expect(state_at_add == 3, "state is stored before the run-list call");
    expect(word(128) == 3 && word(116) == 180, "state 3 and counter 180");
    expect(static_cast<unsigned>(word(12)) == 0xffefffffu, "only bit 20 cleared");

    fresh(4, 1);
    msg_poke__9Containeri(object, 0);
    expect(added == 0 && word(128) == 1 && word(116) == 99, "nonzero state: nothing else changes");
    expect(static_cast<unsigned>(word(12)) == 0xffefffffu, "bit 20 still cleared when state is nonzero");

    fresh(3, 0);
    msg_poke__9Containeri(object, 0);
    expect(added == 0 && word(128) == 0 && word(116) == 99, "without bit 2 nothing happens");
    expect(static_cast<unsigned>(word(12)) == 0xffffffffu, "flag word unchanged without bit 2");

    fresh(0xfffffffb, 0);
    msg_poke__9Containeri(object, 0);
    expect(added == 0 && word(128) == 0, "all other option bits do not matter");
    return failures ? 1 : 0;
}
