#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void* clone__5Chest(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_ctors_ant_lantern_view_44cb98[8] = {};

static const unsigned kSize = 136;
static const unsigned kRanges[][2] = {{12, 30}, {44, 14}, {60, 12}, {76, 2}, {80, 10}, {132, 4}};

static int failures = 0;
static unsigned char* buffer = nullptr;
static int requests = 0;
static bool return_null = false;
static std::vector<void*> heads;
static std::vector<const void*> head_sources;
static bool saw_game_vtable = false;
static bool saw_chest_vtable = false;
static int anim_calls = 0;
static void* anim_dest = nullptr;
static const void* anim_src = nullptr;
static unsigned char at_anim_copy[kSize];

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
extern "C" void __14AnimationStateRC14AnimationState(void* destination, const void* source) {
    ++anim_calls;
    anim_dest = destination;
    anim_src = source;
    saw_chest_vtable = read_word(buffer + 72) == low_pointer(norrath_ctors_ant_lantern_view_44cb98);
    std::memcpy(at_anim_copy, buffer, kSize);
}

int main() {
    unsigned char source[kSize];
    unsigned char created[kSize];
    for (unsigned i = 0; i < kSize; ++i) source[i] = static_cast<unsigned char>(0x10 + i);
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    void* result = clone__5Chest(source);
    expect(result == created, "returns the allocation");
    expect(requests == int(kSize), "allocation size");
    expect(heads.size() == 3u && heads[0] == created && heads[1] == created + 4 && heads[2] == created + 8,
           "copy-constructs the three heads");
    expect(head_sources.size() == 3u && head_sources[0] == source && head_sources[1] == source + 4 &&
               head_sources[2] == source + 8,
           "head sources");
    expect(saw_game_vtable, "the GameObject vtable is stored before the first head");
    expect(anim_calls == 1 && anim_dest == created + 92 && anim_src == source + 92, "animation state copy at +92");
    expect(saw_chest_vtable, "the 5Chest vtable is stored before the animation state copy");
    bool in_range[kSize] = {};
    for (const auto& r : kRanges)
        for (unsigned i = 0; i < r[1]; ++i) in_range[r[0] + i] = true;
    bool fields = true, gaps = true, before_anim = true;
    for (unsigned i = 0; i < kSize; ++i) {
        if (i < 12u || (i >= 72u && i < 76u) || (i >= 92u && i < 132u)) continue;
        if (in_range[i] && created[i] != source[i]) fields = false;
        if (!in_range[i] && created[i] != 0xa5) gaps = false;
        if (in_range[i] && i < 92 && at_anim_copy[i] != source[i]) before_anim = false;
    }
    expect(fields, "copies the payload ranges");
    expect(gaps, "leaves the gap bytes alone");
    expect(before_anim, "the +12..+89 fields are copied before the animation state copy");
    for (unsigned i = 92; i < 132; ++i) expect(created[i] == 0xa5, "the clone itself does not write the animation state bytes");
    for (unsigned i = 0; i < 12u; ++i) expect(created[i] == 0, "head words zeroed by the constructor");
    expect(read_word(created + 72) == low_pointer(norrath_ctors_ant_lantern_view_44cb98), "final vtable");
    expect(at_anim_copy[132] == 0xa5 && at_anim_copy[135] == 0xa5, "word 132 is copied after the animation state copy");
    return_null = true;
    heads.clear();
    anim_calls = 0;
    std::memset(created, 0xa5, sizeof created);
    expect(clone__5Chest(source) == nullptr, "null allocation returns null");
    expect(heads.empty() && anim_calls == 0, "null allocation constructs nothing");
    bool untouched = true;
    for (unsigned i = 0; i < kSize; ++i) if (created[i] != 0xa5) untouched = false;
    expect(untouched, "null allocation writes nothing");
    return failures == 0 ? 0 : 1;
}
