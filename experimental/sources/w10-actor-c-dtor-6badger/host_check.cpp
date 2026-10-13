// Host check only. Not an EE build input.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void dtor_6Badger(void* object, int flags) asm("_$_6Badger");
extern "C" unsigned char norrath_ctors_ant_lantern_view_44c350[8] = {};

static int failures = 0;
static unsigned char* current;
static std::vector<std::string> events;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static const unsigned want_vt = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_ant_lantern_view_44c350));
extern "C" void creature_destructor(void* object, int flags) asm("_$_8Creature");
extern "C" void creature_destructor(void* object, int flags) {
    events.push_back("creature:" + std::string(object == current ? "self" : "other") + ":" + std::to_string(flags) +
                     ":" + (read_word(current + 72) == want_vt ? "vt" : "novt"));
}
extern "C" void norrath_block_free(void* object) { events.push_back(object == current ? "free" : "free-other"); }

int main() {
    const int flags_to_try[] = {0, 1, 2, 3, 4, 5, -1, -2};
    for (int flags : flags_to_try) {
        alignas(16) unsigned char object[400];
        std::memset(object, 0x3c, sizeof object);
        unsigned char before[400];
        std::memcpy(before, object, sizeof object);
        current = object;
        events.clear();
        dtor_6Badger(object, flags);
        std::vector<std::string> want = {"creature:self:0:vt"};
        if (flags & 1) want.push_back("free");
        expect(events == want, "call sequence, base flag 0, vtable stored first, free only for bit 0");
        expect(read_word(object + 72) == want_vt, "final vtable word");
        std::memcpy(before + 72, object + 72, 4);
        expect(std::memcmp(object, before, sizeof object) == 0, "only +72 written");
    }
    return failures ? 1 : 0;
}
