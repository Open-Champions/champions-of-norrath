// Host check only. Not an EE build input. Confirms the wrapper unit exposes the original's symbol
// label for the included implementation and that flag bit 0 and the "name=" removal still behave.
#define NORRATH_GAMEOBJECT_HOST_CHECK 1
#include <cstdint>
#include <initializer_list>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" unsigned long long norrath_gameobject_host_base = 0;
extern "C" unsigned char norrath_constructor_tag_b_view[8] = {};
extern "C" const unsigned char norrath_factory_ctype_minimum_view[256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
extern "C" const unsigned char norrath_game_object_vtable_view[4] = {};
#include "dtor.cpp"

static int failures = 0;
static unsigned char* active_object = nullptr;
static std::vector<std::string> trace;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void put_word(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static unsigned get_word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

extern "C" int norrath_factory_strncmp(const char* left, const char* right, unsigned count) {
    return std::strncmp(left, right, count);
}
extern "C" void norrath_object_remove_string(char* text, void*) { trace.push_back(std::string("remove:") + text); }
extern "C" void norrath_list_destroy(void* head) {
    const unsigned char* h = static_cast<unsigned char*>(head);
    trace.push_back(h == active_object + 8 ? "list:+8" : h == active_object + 4 ? "list:+4" : h == active_object ? "list:+0" : "list:?");
}
extern "C" void norrath_block_free(void* object) { trace.push_back(object == active_object ? "free:object" : "free:?"); }

int main() {
    static unsigned char arena[4096];
    norrath_gameobject_host_base = reinterpret_cast<unsigned long long>(arena);
    std::strcpy(reinterpret_cast<char*>(arena + 0x200), "name=alpha");
    std::strcpy(reinterpret_cast<char*>(arena + 0x220), "hide=no");
    std::strcpy(reinterpret_cast<char*>(arena + 0x240), "name=beta");
    put_word(arena + 0x300, 0x220); put_word(arena + 0x304, 0x200); put_word(arena + 0x308, 0x240); put_word(arena + 0x30c, 0);
    unsigned char object[80];
    active_object = object;
    for (int flags : {0, 1, 2, 3}) {
        std::memset(object, 0x5a, sizeof object);
        put_word(object + 44, 0x300);
        std::memcpy(norrath_constructor_tag_b_view, "NAME\0\0\0", 8);
        trace.clear();
        norrath_vt_dtor_10GameObject(object, flags);
        expect(trace.size() >= 5u && trace[0] == "remove:alpha" && trace[1] == "remove:beta", "both name values are removed in order");
        expect(trace[2] == "list:+8" && trace[3] == "list:+4" && trace[4] == "list:+0", "three list heads destroyed");
        expect((flags & 1) ? (trace.size() == 6u && trace[5] == "free:object") : trace.size() == 5u, "free only when bit 0 is set");
        expect(get_word(object + 72) == static_cast<unsigned>(reinterpret_cast<unsigned long long>(norrath_game_object_vtable_view) - norrath_gameobject_host_base), "vtable stored");
    }
    if (failures == 0) std::printf("_$_10GameObject wrapper cases passed\n");
    return failures == 0 ? 0 : 1;
}
