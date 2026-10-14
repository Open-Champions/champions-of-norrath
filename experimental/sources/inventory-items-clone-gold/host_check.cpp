#include <cstdint>
#include <cstring>
#include <vector>

extern "C" void* clone__4Gold(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_player_view_44cd48[8] = {};

static unsigned char buffer[192];
static int requests;
static bool return_null;
static std::vector<void*> heads;
static std::vector<const void*> head_sources;
static void* anim_destination;
static const void* anim_source;
static unsigned vtable_at_first_head, vtable_at_animation;

extern "C" unsigned char* norrath_block_alloc(int size) {
    ++requests;
    if (size != 192 || return_null) return nullptr;
    std::memset(buffer, 0xa5, sizeof buffer);
    return buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    heads.push_back(destination);
    head_sources.push_back(source);
    if (heads.size() == 1) std::memcpy(&vtable_at_first_head, buffer + 72, 4);
}
extern "C" void __14AnimationStateRC14AnimationState(void* destination, const void* source) {
    anim_destination = destination;
    anim_source = source;
    std::memcpy(&vtable_at_animation, buffer + 72, 4);
}

static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }

int main() {
    unsigned char source[192];
    for (int i = 0; i < 192; ++i) source[i] = static_cast<unsigned char>(i + 1);
    return_null = true;
    if (clone__4Gold(source) != nullptr || requests != 1 || !heads.empty() || anim_destination) return 1;
    return_null = false;
    unsigned char* created = static_cast<unsigned char*>(clone__4Gold(source));
    if (created != buffer || requests != 2) return 2;
    if (heads.size() != 3 || heads[0] != buffer || heads[1] != buffer + 4 || heads[2] != buffer + 8) return 3;
    if (head_sources[0] != source || head_sources[1] != source + 4 || head_sources[2] != source + 8) return 4;
    if (anim_destination != buffer + 88 || anim_source != source + 88) return 5;
    unsigned vt;
    std::memcpy(&vt, buffer + 72, 4);
    if (vt != low(norrath_player_view_44cd48)) return 6;
    if (vtable_at_first_head != low(norrath_game_object_vtable_view)) return 8;   // stored before the head copies
    if (vtable_at_animation != low(norrath_player_view_44cd48)) return 9;         // replaced before the AnimationState copy
    // Ranges that are copied from the source.
    static const int copied[][2] = {{12, 28}, {28, 32}, {32, 36}, {36, 40}, {40, 42}, {44, 58}, {60, 72},
                                    {76, 77}, {80, 88}, {128, 192}};
    bool is_copied[192] = {};
    for (const auto& range : copied) for (int i = range[0]; i < range[1]; ++i) is_copied[i] = true;
    for (int i = 0; i < 192; ++i) {
        if (i >= 72 && i < 76) continue;       // vtable word
        if (i >= 88 && i < 128) continue;      // AnimationState: constructor only
        if (is_copied[i] ? created[i] != source[i] : created[i] != 0xa5) return 100 + i;
    }
    // Bytes 58 and 59 sit between the halfword at 56 and the float at 60.
    if (created[58] != 0xa5 || created[59] != 0xa5) return 7;
    return 0;
}
