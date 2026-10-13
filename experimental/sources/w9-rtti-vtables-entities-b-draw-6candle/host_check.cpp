#include <cstdio>
#include <cstring>
#include <initializer_list>

static int run_calls;
static void* run_seen;
static unsigned char* current_object;
static unsigned char flag_at_call;
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    ++run_calls;
    run_seen = object;
    flag_at_call = current_object[140];
}

#include "draw.cpp"

int main() {
    int failures = 0;
    alignas(8) unsigned char object[256];
    for (unsigned char flag : {0, 1, 0x7f, 0x80, 0xff}) {
        for (unsigned char other : {0, 1, 0xff}) {
            std::memset(object, 0x33, sizeof object);
            object[140] = flag;
            object[141] = other;
            current_object = object;
            run_calls = 0;
            msg_draw__6Candle(object);
            if (object[141] != 0) { std::fprintf(stderr, "fail: byte not cleared\n"); ++failures; }
            if (flag == 0) {
                if (run_calls != 1 || run_seen != object || flag_at_call != 0 || object[140] != 1) { std::fprintf(stderr, "fail: flag 0 path\n"); ++failures; }
            } else if (run_calls != 0 || object[140] != flag) {
                std::fprintf(stderr, "fail: flag %02x path\n", flag); ++failures;
            }
            if (object[140 - 1] != 0x33 || object[141 + 1] != 0x33) { std::fprintf(stderr, "fail: neighbours\n"); ++failures; }
        }
    }
    return failures ? 1 : 0;
}
