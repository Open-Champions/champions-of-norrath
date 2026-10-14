#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags);
extern "C" unsigned long long norrath_gameobject_host_base = 0;
extern "C" unsigned char norrath_constructor_tag_b_view[8] = {};
extern "C" unsigned char norrath_factory_ctype_minimum_view[256] = {};
extern "C" unsigned char norrath_game_object_vtable_view[4] = {};

static int failures = 0;
static unsigned char* active_object = nullptr;
static bool swap_on_compare = false;
static unsigned swap_word = 0;
static bool bad_compare = false;
static std::vector<std::string> trace;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" int norrath_factory_strncmp(const char* left, const char* right, unsigned count) {
    if (count != 4u || std::strcmp(right, "name") != 0) bad_compare = true;
    if (swap_on_compare) {
        swap_on_compare = false;
        write_word(active_object + 44, swap_word);
    }
    for (unsigned i = 0; i < count; ++i) {
        const unsigned char a = static_cast<unsigned char>(left[i]);
        const unsigned char b = static_cast<unsigned char>(right[i]);
        if (a != b) return int(a) - int(b);
        if (a == 0u) return 0;
    }
    return 0;
}
extern "C" void norrath_object_remove_string(char* text, void* object) {
    (void)object;
    if (trace.size() > 8u) {
        write_word(active_object + 44, 0u);
        return;
    }
    trace.push_back(std::string("remove:") + (text ? text : "<null>"));
}
extern "C" void norrath_list_destroy(void* head) {
    const auto base = reinterpret_cast<unsigned char*>(active_object);
    const auto got = reinterpret_cast<unsigned char*>(head);
    if (got == base + 8) trace.push_back("list:+8");
    else if (got == base + 4) trace.push_back("list:+4");
    else if (got == base) trace.push_back("list:+0");
    else trace.push_back("list:?");
}
extern "C" void norrath_block_free(void* object) {
    trace.push_back(object == active_object ? "free:object" : "free:?");
}

static void reset_tag(const char* text) {
    std::memset(norrath_constructor_tag_b_view, 0, 8);
    std::memcpy(norrath_constructor_tag_b_view, text, std::strlen(text) + 1u);
}

static unsigned vtable_word() {
    return static_cast<unsigned>(reinterpret_cast<unsigned long long>(norrath_game_object_vtable_view) -
                                 norrath_gameobject_host_base);
}

int main() {
    alignas(256) static unsigned char arena[4096];
    norrath_gameobject_host_base = reinterpret_cast<unsigned long long>(arena);
    std::memset(norrath_factory_ctype_minimum_view, 0, 256);
    for (int ch = 'A'; ch <= 'Z'; ++ch) norrath_factory_ctype_minimum_view[ch + 128] = 1;
    for (int ch = 'a'; ch <= 'z'; ++ch) norrath_factory_ctype_minimum_view[ch + 128] = 2;

    auto place = [&](unsigned offset, const char* text) {
        std::strcpy(reinterpret_cast<char*>(arena + offset), text);
    };
    place(0x200, "hide=no");
    place(0x220, "name=alpha");
    place(0x240, "named=no");
    place(0x260, "name=beta");
    place(0x280, "NAME=nope");
    write_word(arena + 0x300, 0x200);
    write_word(arena + 0x304, 0x220);
    write_word(arena + 0x308, 0x240);
    write_word(arena + 0x30c, 0x260);
    write_word(arena + 0x310, 0);
    write_word(arena + 0x320, 0x280);
    write_word(arena + 0x324, 0);
    write_word(arena + 0x340, 0x220);
    write_word(arena + 0x344, 0);
    write_word(arena + 0x360, 0x260);
    write_word(arena + 0x364, 0);

    unsigned char object[80];
    active_object = object;

    std::memset(object, 0x5a, sizeof object);
    write_word(object + 44, 0x300);
    reset_tag("NAME");
    trace.clear();
    bad_compare = false;
    norrath_vt_dtor_10GameObject(object, 0);
    expect(!bad_compare, "compares four bytes with the folded tag");
    expect(std::strcmp(reinterpret_cast<char*>(norrath_constructor_tag_b_view), "name") == 0, "tag folded in place");
    expect(trace.size() == 5u && trace[0] == "remove:alpha" && trace[1] == "remove:beta" &&
               trace[2] == "list:+8" && trace[3] == "list:+4" && trace[4] == "list:+0",
           "removes both name values then destroys three heads");
    expect(read_word(object + 72) == vtable_word(), "vtable stored at object+72");

    std::memset(object, 0, sizeof object);
    write_word(object + 44, 0x320);
    reset_tag("NAME");
    trace.clear();
    norrath_vt_dtor_10GameObject(object, 2);
    expect(trace.size() == 3u && trace[0] == "list:+8", "uppercase argument is not a name value");

    std::memset(object, 0, sizeof object);
    write_word(object + 44, 0);
    reset_tag("name");
    trace.clear();
    norrath_vt_dtor_10GameObject(object, 1);
    expect(trace.size() == 4u && trace[0] == "list:+8" && trace[3] == "free:object",
           "missing arguments still destroy heads and honor flag bit 0");

    std::memset(object, 0, sizeof object);
    write_word(object + 44, 0x340);
    reset_tag("name");
    swap_on_compare = true;
    swap_word = 0x360;
    trace.clear();
    norrath_vt_dtor_10GameObject(object, 0);
    expect(trace.size() == 4u && trace[0] == "remove:beta",
           "the value is reread from object+44 after the compare");
    return failures == 0 ? 0 : 1;
}
