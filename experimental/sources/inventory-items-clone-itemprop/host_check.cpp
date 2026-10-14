#include <cstdint>
#include <cstring>
#include <vector>

extern "C" void* clone__8ItemProp(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_player_view_44d018[8] = {};
extern "C" unsigned char norrath_player_view_44cef8[8] = {};

static unsigned char buffer[400];
static int requests;
static bool return_null;
static std::vector<void*> heads;
static std::vector<const void*> head_sources;
static unsigned vtable_at_first_head;

extern "C" unsigned char* norrath_block_alloc(int size) {
    ++requests;
    if (size != 400 || return_null) return nullptr;
    std::memset(buffer, 0xa5, sizeof buffer);
    return buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    heads.push_back(destination);
    head_sources.push_back(source);
    if (heads.size() == 1) std::memcpy(&vtable_at_first_head, buffer + 72, 4);
}

static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }

int main() {
    unsigned char source[400];
    for (int i = 0; i < 400; ++i) source[i] = static_cast<unsigned char>(i * 7 + 1);
    return_null = true;
    if (clone__8ItemProp(source) != nullptr || requests != 1 || !heads.empty()) return 1;
    return_null = false;
    unsigned char* created = static_cast<unsigned char*>(clone__8ItemProp(source));
    if (created != buffer || requests != 2) return 2;
    if (heads.size() != 3 || heads[0] != buffer || heads[1] != buffer + 4 || heads[2] != buffer + 8) return 3;
    if (head_sources[0] != source || head_sources[1] != source + 4 || head_sources[2] != source + 8) return 4;
    if (vtable_at_first_head != low(norrath_game_object_vtable_view)) return 5;
    unsigned vt;
    std::memcpy(&vt, buffer + 72, 4);
    if (vt != low(norrath_player_view_44cef8)) return 6;
    // Ranges copied from the source; everything else except the vtable word keeps the allocator fill.
    static const int copied[][2] = {{12, 42}, {44, 58}, {60, 72}, {76, 228}, {232, 322}, {328, 400}};
    bool is_copied[400] = {};
    for (const auto& range : copied) for (int i = range[0]; i < range[1]; ++i) is_copied[i] = true;
    for (int i = 0; i < 400; ++i) {
        if (i >= 72 && i < 76) continue;
        if (is_copied[i] ? created[i] != source[i] : created[i] != 0xa5) return 100 + i;
    }
    return 0;
}
