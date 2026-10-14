#include "clone.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" const unsigned char norrath_factory_animation_vtable[8] = {};
extern "C" const unsigned char norrath_game_object_vtable_view[8] = {};

static int failures = 0;
static unsigned char* buffer = nullptr;
static int requests = 0;
static bool return_null = false;
static int heads = 0;
static unsigned head_offsets[3];
static int state_calls = 0;
static unsigned state_dest = 0;
static unsigned state_source = 0;
static bool before_done = false;
static bool after_untouched = false;
static const unsigned char* source_base = nullptr;
extern "C" void __14AnimationStateRC14AnimationState(void* destination, const void* source) {
    ++state_calls;
    state_dest = static_cast<unsigned>(static_cast<unsigned char*>(destination) - buffer);
    state_source = static_cast<unsigned>(static_cast<const unsigned char*>(source) - source_base);
    before_done = std::memcmp(buffer + 88, source_base + 88, 4) == 0;
    after_untouched = buffer[140] == 0xa5 && buffer[140+3] == 0xa5;
}

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    requests = requested_size;
    return return_null ? nullptr : buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    (void)source;
    auto* dest = static_cast<unsigned char*>(destination);
    head_offsets[heads < 3 ? heads : 2] = static_cast<unsigned>(dest - buffer);
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
    ++heads;
}

int main() {
    const unsigned size = 152u;
    unsigned char source[152];
    unsigned char created[152];
    for (unsigned i = 0; i < size; ++i) source[i] = static_cast<unsigned char>(0x10 + i);
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    source_base = source;
    void* result = clone__16CosmeticPropAnim(source);
    expect(result == created, "returns the allocation");
    expect(requests == 152, "allocation size");
    expect(heads == 3 && head_offsets[0] == 0 && head_offsets[1] == 4 && head_offsets[2] == 8, "three heads at 0, 4, 8");
    expect(state_calls == 1 && state_dest == 100 && state_source == 100, "one state copy at +100");
    expect(before_done, "word +88 written before the state copy");
    expect(after_untouched, "fields from +140 written after the state copy");
    static const unsigned ranges[][2] = {{12, 4}, {16, 8}, {24, 4}, {28, 4}, {36, 4}, {32, 4}, {40, 1}, {41, 1}, {52, 4}, {48, 4}, {76, 4}, {60, 4}, {80, 4}, {44, 4}, {56, 2}, {64, 4}, {68, 4}, {84, 4}, {92, 4}, {96, 4}, {88, 4}, {140, 1}, {144, 4}, {148, 4}};
    bool covered[152] = {};
    for (unsigned i = 0; i < 12; ++i) { covered[i] = true; expect(created[i] == 0, "list head word zeroed"); }
    for (const auto& r : ranges) {
        for (unsigned i = r[0]; i < r[0] + r[1]; ++i) {
            covered[i] = true;
            expect(created[i] == source[i], "copied byte");
        }
    }
    covered[72] = covered[73] = covered[74] = covered[75] = true;
    expect(read_word(created + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_factory_animation_vtable)), "class table at +72");
    for (unsigned i = 0; i < size; ++i) if (!covered[i]) expect(created[i] == 0xa5, "gap byte untouched");
    return_null = true;
    std::memset(created, 0xa5, sizeof created);
    heads = 0;
    state_calls = 0;
    expect(clone__16CosmeticPropAnim(source) == nullptr, "null allocation returns null");
    expect(heads == 0 && state_calls == 0, "no list heads or state copy on null");
    for (unsigned char b : created) expect(b == 0xa5, "null path writes nothing");
    return failures ? 1 : 0;
}
