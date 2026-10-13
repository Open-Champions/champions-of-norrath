#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void* clone__10BlackWidow(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_actor_view_4488f0[8] = {};
extern "C" unsigned char norrath_ctors_ant_lantern_view_44c260[8] = {};

static const unsigned kSize = 1040;
static const unsigned kRanges[][2] = {{12, 42}, {44, 58}, {60, 72}, {76, 77}, {80, 86}, {88, 157}, {160, 174}, {176, 190}, {192, 206}, {208, 234}, {236, 286}, {552, 571}, {572, 574}, {580, 750}, {756, 801}, {804, 888}, {892, 1004}, {1008, 1036}};
static const unsigned kHeads[] = {0, 4, 8, 576, 752, 888, 1004};

static int failures = 0;
static unsigned char* buffer = nullptr;
static int requests = 0;
static bool return_null = false;
static std::vector<std::string> events;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
static std::string vt() {
    unsigned w = read_word(buffer + 72);
    if (w == low_pointer(norrath_game_object_vtable_view)) return "object";
    if (w == low_pointer(norrath_actor_view_4488f0)) return "creature";
    if (w == low_pointer(norrath_ctors_ant_lantern_view_44c260)) return "blackwidow";
    return "none";
}
static const unsigned char* source_base;
extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    requests = requested_size;
    if (return_null) return nullptr;
    return buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    auto* dest = static_cast<unsigned char*>(destination);
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
    unsigned off = static_cast<unsigned>(dest - buffer);
    events.push_back("head:" + std::to_string(off) + ":" + (static_cast<const unsigned char*>(source) - source_base == off ? "same" : "wrong") + ":" + vt());
}
extern "C" void __15AnimationState8RC15AnimationState8(void* destination, const void* source) {
    unsigned off = static_cast<unsigned>(static_cast<unsigned char*>(destination) - buffer);
    events.push_back("state:" + std::to_string(off) + ":" + (static_cast<const unsigned char*>(source) - source_base == off ? "same" : "wrong") + ":" + vt());
}

int main() {
    unsigned char source[kSize];
    unsigned char created[kSize];
    for (unsigned i = 0; i < kSize; ++i) source[i] = static_cast<unsigned char>(0x10 + i * 3);
    std::memset(created, 0xa5, sizeof created);
    for (unsigned i = 0; i < kSize; ++i) if (source[i] == 0xa5 || source[i] == 0) source[i] = 0x11;
    buffer = created;
    source_base = source;
    void* result = clone__10BlackWidow(source);
    expect(result == created, "returns the allocation");
    expect(requests == int(kSize), "allocation size");
    std::vector<std::string> want = {"head:0:same:object", "head:4:same:object", "head:8:same:object", "state:288:same:creature", "head:576:same:creature", "head:752:same:creature", "head:888:same:creature", "head:1004:same:blackwidow"};
    expect(events == want, "constructor calls with the table in force at each call");
    if (events != want) for (auto& e : events) std::fprintf(stderr, "  got %s\n", e.c_str());
    bool in_range[kSize] = {};
    for (const auto& r : kRanges)
        for (unsigned i = r[0]; i < r[1]; ++i) in_range[i] = true;
    bool fields = true, gaps = true;
    for (unsigned i = 0; i < kSize; ++i) {
        bool is_head = false;
        for (unsigned h : kHeads) if (i >= h && i < h + 4) is_head = true;
        if (is_head || (i >= 72u && i < 76u)) continue;
        if (in_range[i] && created[i] != source[i]) fields = false;
        if (!in_range[i] && created[i] != 0xa5) gaps = false;
    }
    expect(fields, "copies the payload ranges");
    expect(gaps, "leaves the gap bytes alone (including the animation state area)");
    for (unsigned h : kHeads) for (unsigned i = 0; i < 4; ++i) expect(created[h + i] == 0, "head words zeroed by the constructor");
    expect(vt() == "blackwidow", "final vtable");
    return_null = true;
    events.clear();
    std::memset(created, 0xa5, sizeof created);
    expect(clone__10BlackWidow(source) == nullptr, "null allocation returns null");
    expect(events.empty(), "null allocation constructs nothing");
    bool untouched = true;
    for (unsigned i = 0; i < kSize; ++i) if (created[i] != 0xa5) untouched = false;
    expect(untouched, "null allocation writes nothing");
    return failures == 0 ? 0 : 1;
}
