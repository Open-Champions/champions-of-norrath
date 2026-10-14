#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void* clone__8Creature(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_actor_view_4488f0[8] = {};


static int failures = 0;
static unsigned char* buffer = nullptr;
static unsigned char* source_bytes = nullptr;
static int requests = 0;
static bool return_null = false;
static std::vector<std::string> calls;
static bool game_vtable_first = false;
static bool creature_vtable_at_animation = false;
static bool animation_was_untouched = false;

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
extern "C" void norrath_list_head_copy_construct(void* destination, const void*) {
    auto* dest = static_cast<unsigned char*>(destination);
    if (calls.empty()) game_vtable_first = read_word(buffer + 72) == low_pointer(norrath_game_object_vtable_view);
    unsigned offset = static_cast<unsigned>(dest - buffer);
    calls.push_back("list-" + std::to_string(offset));
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
}
extern "C" void animation_state_copy(void* destination, const void* source) asm("__15AnimationState8RC15AnimationState8");
extern "C" void animation_state_copy(void* destination, const void* source) {
    calls.push_back("anim");
    expect(destination == buffer + 288 && source == source_bytes + 288, "animation copy uses offset 288");
    creature_vtable_at_animation = read_word(buffer + 72) == low_pointer(norrath_actor_view_4488f0);
    animation_was_untouched = buffer[288] == 0xa5 && buffer[400] == 0xa5 && buffer[551] == 0xa5;
    buffer[288] = 0x11;
    buffer[551] = 0x11;
}

static const unsigned spans[][2] = {
    {12, 30}, {44, 14}, {60, 12}, {76, 1}, {80, 6}, {88, 69}, {160, 14}, {176, 14}, {192, 14}, {208, 26}, {236, 50}, {552, 19}, {572, 2}, {580, 170}, {756, 45}, {804, 84}, {892, 68}
};

int main() {
    unsigned char source[960];
    unsigned char created[960];
    for (unsigned i = 0; i < 960u; ++i) source[i] = static_cast<unsigned char>(0x40 + (i * 3u));
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    source_bytes = source;
    return_null = false;
    calls.clear();
    void* result = clone__8Creature(source);
    expect(result == created, "returns the allocation");
    expect(requests == 960, "allocates 960 bytes");
    const char* wanted[] = {"list-0", "list-4", "list-8", "anim", "list-576", "list-752", "list-888"};
    expect(calls.size() == 7, "seven constructor calls");
    for (unsigned i = 0; i < 7 && i < calls.size(); ++i) expect(calls[i] == wanted[i], wanted[i]);
    expect(game_vtable_first, "GameObject vtable is visible to the first list");
    expect(creature_vtable_at_animation, "Creature vtable is visible to the animation copy");
    expect(animation_was_untouched, "animation bytes are not copied before the constructor");
    expect(created[288] == 0x11 && created[551] == 0x11 && created[400] == 0xa5, "animation region stays with the constructor");
    expect(created[42] == 0xa5 && created[43] == 0xa5 && created[58] == 0xa5 && created[59] == 0xa5, "GameObject holes stay untouched");
    expect(created[41] == source[41] && created[68] == source[68], "copied GameObject payload");
    expect(created[76] == source[76] && created[77] == 0xa5 && created[79] == 0xa5, "byte 76 is copied and 77..79 are not");
    expect(created[80] == source[80] && created[85] == source[85] && created[86] == 0xa5 && created[87] == 0xa5,
           "bytes 80..85 are copied and 86..87 are not");
    expect(created[572] == source[572] && created[573] == source[573] && created[571] == 0xa5 && created[574] == 0xa5,
           "the half at 572 is copied between holes");
    bool spans_ok = true;
    bool copied[960] = {};
    for (const unsigned* span : spans) {
        for (unsigned i = 0; i < span[1]; ++i) copied[span[0] + i] = true;
    }
    bool head_byte[960] = {};
    const unsigned heads[] = {0, 4, 8, 576, 752, 888};
    for (unsigned head : heads) {
        for (unsigned i = 0; i < 4u; ++i) head_byte[head + i] = true;
    }
    for (unsigned i = 0; i < 960u; ++i) {
        if (i == 288 || i == 551 || i == 400 || (i >= 72u && i < 76u)) continue;
        if (head_byte[i]) {
            if (created[i] != 0) spans_ok = false;
        } else if (copied[i]) {
            if (created[i] != source[i]) spans_ok = false;
        } else if (created[i] != 0xa5) spans_ok = false;
    }
    expect(spans_ok, "copied spans match and every other byte stays as allocated");
    expect(read_word(created + 72) == low_pointer(norrath_actor_view_4488f0), "final vtable");
    expect(created[892] == source[892] && created[959] == source[959], "tail copies through 959");
    return_null = true;
    calls.clear();
    expect(clone__8Creature(source) == nullptr, "null allocation returns null");
    expect(calls.empty(), "null allocation does not construct");
    return failures == 0 ? 0 : 1;
}
