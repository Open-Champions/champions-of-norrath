#include <cstdio>
#include <cstring>
#include <initializer_list>

static int run_calls;
static void* run_seen;
static unsigned seen_state;
static unsigned char* current_object;
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    ++run_calls;
    run_seen = object;
    std::memcpy(&seen_state, current_object + 316, 4);
}

#include "msg_hurt.cpp"

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
    for (unsigned state : {0u, 1u, 2u, 3u, 4u, 5u}) {
        for (unsigned flags : {0u, 1u, 2u, 0xfffffffeu, 0xffffffffu}) {
            std::memset(object, 0x44, sizeof object);
            std::memcpy(object + 316, &state, 4);
            std::memcpy(object + 292, &flags, 4);
            current_object = object;
            run_calls = 0;
            msg_hurt__13AutoContainerfP10GameObjecti(object, 2.5f, nullptr, 7);
            bool acts = state != 1 && state != 4 && (flags & 1);
            if (acts) {
                expect(run_calls == 1 && run_seen == object, "adds to the run list once");
                expect(seen_state == state, "state still old at the call");
                expect(word(object, 316) == 1 && word(object, 304) == 0, "sets state 1 and counter 0");
            } else {
                expect(run_calls == 0, "no call");
                expect(word(object, 316) == state && word(object, 304) == 0x44444444u, "state and counter untouched");
            }
            expect(word(object, 12) == 0x44444444u, "flag word untouched");
        }
    }
    return failures ? 1 : 0;
}
