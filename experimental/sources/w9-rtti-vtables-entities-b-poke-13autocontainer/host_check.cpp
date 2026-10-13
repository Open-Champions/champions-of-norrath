#include <cstdio>
#include <cstring>
#include <initializer_list>

static int run_calls;
static void* run_seen;
static unsigned seen_state, seen_flags;
static unsigned char* current_object;
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    ++run_calls;
    run_seen = object;
    std::memcpy(&seen_state, current_object + 316, 4);
    std::memcpy(&seen_flags, current_object + 12, 4);
}

#include "msg_poke.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static unsigned word(const unsigned char* base, unsigned off) {
    unsigned v;
    std::memcpy(&v, base + off, 4);
    return v;
}

int main() {
    alignas(8) unsigned char object[512];
    for (unsigned state : {0u, 1u, 3u, 0x80000000u}) {
        for (unsigned flags : {0u, 4u, 0xfffffffbu, 0xffffffffu}) {
            std::memset(object, 0x44, sizeof object);
            unsigned all = 0xffffffffu;
            std::memcpy(object + 12, &all, 4);
            std::memcpy(object + 316, &state, 4);
            std::memcpy(object + 292, &flags, 4);
            current_object = object;
            run_calls = 0;
            msg_poke__13AutoContaineri(object, 12345);
            if (!(flags & 4)) {
                expect(run_calls == 0 && word(object, 12) == all && word(object, 316) == state, "bit 2 clear: nothing changes");
                continue;
            }
            expect(word(object, 12) == 0xffefffffu, "clears bit 20");
            if (state == 0) {
                expect(run_calls == 1 && run_seen == object, "adds to the run list once");
                expect(seen_state == 3 && seen_flags == 0xffefffffu, "state 3 and flag cleared before the call");
                expect(word(object, 316) == 3 && word(object, 304) == 180, "state 3 and counter 180");
            } else {
                expect(run_calls == 0 && word(object, 316) == state && word(object, 304) == 0x44444444u, "state nonzero: no change");
            }
        }
    }
    return failures ? 1 : 0;
}
