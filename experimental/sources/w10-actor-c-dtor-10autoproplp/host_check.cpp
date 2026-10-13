// Host check only. Not an EE build input.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void dtor_10AutoPropLP(void* object, int flags) asm("_$_10AutoPropLP");
extern "C" unsigned char norrath_ctors_ant_lantern_view_44ec40[8] = {};
extern "C" unsigned char norrath_rtti_entities_a_view_44ed78[8] = {};
extern "C" unsigned char norrath_actors_b_view_4844a0[4] = {0x40, 0x30, 0x20, 0x00};
extern "C" void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");

static int failures = 0;
static unsigned char* current;
static std::vector<std::string> events;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static std::string vt() {
    unsigned w = read_word(current + 72);
    if (w == word_of(norrath_ctors_ant_lantern_view_44ec40)) return "8AutoProp";
    if (w == word_of(norrath_rtti_entities_a_view_44ed78)) return "10AutoObject";
    return "none";
}
extern "C" int worldReleaseDoor__FP12_worldHeaderi(void* world, int index) {
    events.push_back("release:" + std::to_string(word_of(world)) + ":" + std::to_string(index) + ":" + vt());
    return 0;
}
extern "C" void animation_state_destructor(void* state, int flags) {
    events.push_back("anim:" + std::string(state == current + 152 ? "+152" : "other") + ":" + std::to_string(flags) + ":" + vt());
}
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    events.push_back("base:" + std::string(object == current ? "self" : "other") + ":" + std::to_string(flags) + ":" + vt());
}
extern "C" void norrath_block_free(void* object) { events.push_back(object == current ? "free" : "free-other"); }

int main() {
    const int flags_to_try[] = {0, 1, 2, 3, 4, 5, -1, -2};
    const int indexes[] = {-1, 0, 7, 0x7fffffff};
    for (int flags : flags_to_try)
        for (int index : indexes) {
            alignas(16) unsigned char object[512];
            std::memset(object, 0x3c, sizeof object);
            const unsigned stored = static_cast<unsigned>(index);
            std::memcpy(object + 396, &stored, 4);
            unsigned char before[512];
            std::memcpy(before, object, sizeof object);
            current = object;
            events.clear();
            dtor_10AutoPropLP(object, flags);
            std::vector<std::string> want = {"release:" + std::to_string(0x00203040u) + ":" + std::to_string(index) + ":8AutoProp",
                                             "anim:+152:2:10AutoObject", "base:self:0:10AutoObject"};
            if (flags & 1) want.push_back("free");
            expect(events == want, "call sequence with the vtable state at each call");
            expect(vt() == "10AutoObject", "final vtable word");
            std::memcpy(before + 72, object + 72, 4);
            expect(std::memcmp(object, before, sizeof object) == 0, "only +72 written");
        }
    return failures ? 1 : 0;
}
