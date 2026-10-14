#include "dtor.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" const unsigned char norrath_player_view_44d2e8[8] = {};

static int failures = 0;
static std::vector<int> trace;
static void* base_object = nullptr;
static int base_flags = -1;
static void* freed = nullptr;
static void* state_object = nullptr;
static int state_flags = -1;
static unsigned stored_at_base = 0;
static unsigned char* watched = nullptr;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    trace.push_back(2);
    base_object = object;
    base_flags = flags;
    stored_at_base = read_word(watched + 72);
}
extern "C" void norrath_block_free(void* object) {
    trace.push_back(3);
    freed = object;
}
extern "C" void animation_state_destroy(void* state, int flags) asm("_$_14AnimationState");
extern "C" void animation_state_destroy(void* state, int flags) {
    trace.push_back(1);
    state_object = state;
    state_flags = flags;
}

int main() {
    unsigned char object[160];
    for (int flags = 0; flags < 4; ++flags) {
        std::memset(object, 0x6b, sizeof object);
        watched = object;
        trace.clear();
        base_object = freed = state_object = nullptr;
        base_flags = state_flags = -1;
        destroy_7Player1(object, flags);
        expect(read_word(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_player_view_44d2e8)), "class table stored at +72");
        expect(stored_at_base == read_word(object + 72), "table stored before the base destructor");
        expect(base_object == object && base_flags == 0, "base destructor on the object with flag 0");
        expect(state_object == nullptr, "no animation state in this class");
        if ((flags & 1) != 0) expect(freed == object && trace.back() == 3, "bit 0 frees last");
        else expect(freed == nullptr && trace.back() == 2, "no free without bit 0");
        for (int i = 0; i < 160; ++i) if (i < 72 || i > 75) expect(object[i] == 0x6b, "no other byte written here");
    }
    return failures == 0 ? 0 : 1;
}
