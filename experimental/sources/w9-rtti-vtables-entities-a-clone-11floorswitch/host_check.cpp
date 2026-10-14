#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void* clone__11FloorSwitch(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_actors_a_view_449790[8] = {};

static const unsigned kSize = 88;
static const unsigned kRanges[][2] = {{12,30}, {44,14}, {60,12}, {76,5}, {84,4}};

static int failures = 0;
static unsigned char* buffer = nullptr;
static int requests = 0;
static bool return_null = false;
static std::vector<void*> heads;
static std::vector<const void*> head_sources;
static bool saw_game_vtable = false;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    requests = requested_size;
    if (return_null) return nullptr;
    return buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    auto* dest = static_cast<unsigned char*>(destination);
    if (heads.empty()) saw_game_vtable = read_word(buffer + 72) == low_pointer(norrath_game_object_vtable_view);
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
    heads.push_back(destination);
    head_sources.push_back(source);
}

int main() {
    unsigned char source[kSize];
    unsigned char created[kSize];
    for (unsigned i = 0; i < kSize; ++i) source[i] = static_cast<unsigned char>(0x10 + i);
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    heads.clear();
    void* result = clone__11FloorSwitch(source);
    expect(result == created, "returns the allocation");
    expect(requests == int(kSize), "allocation size");
    expect(heads.size() == 3u && heads[0] == created && heads[1] == created + 4 && heads[2] == created + 8,
           "copy-constructs the three heads");
    expect(head_sources.size() == 3u && head_sources[0] == source && head_sources[1] == source + 4 &&
               head_sources[2] == source + 8,
           "head sources");
    expect(saw_game_vtable, "the GameObject vtable is stored before the first head");
    bool in_range[kSize] = {};
    for (const auto& r : kRanges)
        for (unsigned i = 0; i < r[1]; ++i) in_range[r[0] + i] = true;
    bool fields = true;
    bool gaps = true;
    for (unsigned i = 0; i < kSize; ++i) {
        if (i < 12u || (i >= 72u && i < 76u)) continue;
        if (in_range[i] && created[i] != source[i]) fields = false;
        if (!in_range[i] && created[i] != 0xa5) gaps = false;
    }
    expect(fields, "copies the payload ranges");
    expect(gaps, "leaves the gap bytes alone");
    for (unsigned i = 0; i < 12u; ++i) expect(created[i] == 0, "head words zeroed by the constructor");
    expect(read_word(created + 72) == low_pointer(norrath_actors_a_view_449790), "final vtable");
    return_null = true;
    heads.clear();
    std::memset(created, 0xa5, sizeof created);
    expect(clone__11FloorSwitch(source) == nullptr, "null allocation returns null");
    expect(heads.empty(), "null allocation does not construct heads");
    bool untouched = true;
    for (unsigned i = 0; i < kSize; ++i) if (created[i] != 0xa5) untouched = false;
    expect(untouched, "null allocation writes nothing");
    return failures == 0 ? 0 : 1;
}
