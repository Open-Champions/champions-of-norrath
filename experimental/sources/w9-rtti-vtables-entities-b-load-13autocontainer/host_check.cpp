#include <cstdio>
#include <cstring>
#include <initializer_list>

static int run_calls;
static void* run_seen;
static unsigned seen_flags, seen_counter, seen_state;
static unsigned char* current_object;
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    ++run_calls;
    run_seen = object;
    std::memcpy(&seen_flags, current_object + 12, 4);
    std::memcpy(&seen_counter, current_object + 304, 4);
    std::memcpy(&seen_state, current_object + 316, 4);
}

#include "load.cpp"

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
    alignas(8) char stream[16];
    for (unsigned state : {0u, 1u, 2u, 3u, 4u, 0xffffffffu}) {
        std::memset(object, 0x33, sizeof object);
        unsigned flags = 0xffffffffu;
        std::memcpy(object + 12, &flags, 4);
        current_object = object;
        std::memset(stream, 0x66, sizeof stream);
        std::memcpy(stream, &state, 4);
        run_calls = 0;
        int used = msg_load__13AutoContainerPc(object, stream);
        expect(used == 4, "consumes four bytes");
        expect(word(object, 316) == state, "stores the stream word at +316");
        if (state == 2) {
            expect(run_calls == 1 && run_seen == object, "adds the object to the run list once");
            expect(seen_flags == 0xffefffffu && word(object, 12) == 0xffefffffu, "clears only bit 20");
            expect(seen_counter == 180 && word(object, 304) == 180, "counter is 180 before the call");
            expect(seen_state == 2, "state stored before the call");
        } else {
            expect(run_calls == 0, "no run-list call for other states");
            expect(word(object, 12) == 0xffffffffu && word(object, 304) == 0x33333333u, "flags and counter untouched");
        }
        expect(object[320] == 0x33 && object[315] == 0x33, "neighbouring bytes untouched");
    }
    return failures ? 1 : 0;
}
