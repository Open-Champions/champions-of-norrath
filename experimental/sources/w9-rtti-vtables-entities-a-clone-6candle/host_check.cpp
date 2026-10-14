// Host check only. Not an EE build input.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void* clone__6Candle(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_actors_a_view_44a9c0[8] = {};

static const unsigned kSize = 148;
static const unsigned kRanges[][2] = {{12, 30}, {44, 14}, {60, 12}, {76, 60}, {136, 6}, {144, 4}};

static int failures = 0;
static unsigned char* buffer = nullptr;
static int requests = 0;
static bool return_null = false;
static std::vector<void*> heads;
static std::vector<const void*> head_sources;
static bool saw_game_vtable = false;
static int light_calls = 0;
static unsigned char* light_arg = nullptr;
static unsigned light_word0 = 0, light_word1 = 0, light_vtable = 0;

static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    requests = requested_size;
    return return_null ? nullptr : buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    auto* dest = static_cast<unsigned char*>(destination);
    if (heads.empty()) saw_game_vtable = read_word(buffer + 72) == low_pointer(norrath_game_object_vtable_view);
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
    heads.push_back(destination);
    head_sources.push_back(source);
}
extern "C" void lightAdd__FP5Light(void* light) {
    ++light_calls;
    light_arg = static_cast<unsigned char*>(light);
    light_word0 = read_word(light_arg);
    light_word1 = read_word(light_arg + 4);
    light_vtable = read_word(buffer + 72);
}

static void run_case(const char* label, unsigned word0, unsigned word1) {
    unsigned char source[kSize];
    unsigned char created[kSize];
    for (unsigned i = 0; i < kSize; ++i) source[i] = static_cast<unsigned char>(0x10 + i);
    std::memcpy(source + 76, &word0, 4);
    std::memcpy(source + 80, &word1, 4);
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    heads.clear(); head_sources.clear();
    light_calls = 0; light_arg = nullptr;
    void* result = clone__6Candle(source);
    expect(result == created && requests == int(kSize), label);
    expect(heads.size() == 3u && heads[0] == created && heads[1] == created + 4 && heads[2] == created + 8, "copy-constructs the three heads");
    expect(head_sources.size() == 3u && head_sources[1] == source + 4 && head_sources[2] == source + 8 && head_sources[0] == source, "head sources");
    expect(saw_game_vtable, "the GameObject vtable is stored before the first head");
    bool in_range[kSize] = {};
    for (const auto& r : kRanges)
        for (unsigned i = 0; i < r[1]; ++i) in_range[r[0] + i] = true;
    for (unsigned i = 12; i < kSize; ++i) {
        if (i >= 72u && i < 76u) continue;
        const bool light_word = i >= 76u && i < 84u;
        if (light_word) continue;  // checked below
        if (in_range[i] && created[i] != source[i]) { std::fprintf(stderr, "copy byte %u (%s)\n", i, label); ++failures; }
        if (!in_range[i] && created[i] != 0xa5) { std::fprintf(stderr, "gap byte %u (%s)\n", i, label); ++failures; }
    }
    for (unsigned i = 0; i < 12u; ++i) expect(created[i] == 0, "head words zeroed by the constructor");
    expect(read_word(created + 72) == low_pointer(norrath_actors_a_view_44a9c0), "final vtable");
    const bool add = word0 != 0 || word1 != 0;
    expect(light_calls == (add ? 1 : 0), "lightAdd only when either word is nonzero");
    if (add) {
        expect(light_arg == created + 76 && light_word0 == 0 && light_word1 == 0, "words cleared before lightAdd, argument is the member");
        expect(light_vtable == low_pointer(norrath_actors_a_view_44a9c0), "class vtable is in place before lightAdd");
        expect(read_word(created + 76) == 0 && read_word(created + 80) == 0, "words stay cleared");
    } else {
        expect(read_word(created + 76) == 0 && read_word(created + 80) == 0, "zero words copied");
    }
    for (unsigned i = 84; i < 136; ++i) expect(created[i] == source[i], "light body copied");
}

int main() {
    run_case("both zero", 0, 0);
    run_case("first nonzero", 5, 0);
    run_case("second nonzero", 0, 7);
    run_case("both nonzero", 0xffffffffu, 0x80000000u);
    unsigned char source[kSize] = {};
    unsigned char created[kSize];
    return_null = true;
    heads.clear(); light_calls = 0;
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    expect(clone__6Candle(source) == nullptr, "null allocation returns null");
    expect(heads.empty() && light_calls == 0, "null allocation constructs nothing");
    bool untouched = true;
    for (unsigned i = 0; i < kSize; ++i) if (created[i] != 0xa5) untouched = false;
    expect(untouched, "null allocation writes nothing");
    if (failures == 0) std::printf("clone__6Candle cases passed\n");
    return failures == 0 ? 0 : 1;
}
