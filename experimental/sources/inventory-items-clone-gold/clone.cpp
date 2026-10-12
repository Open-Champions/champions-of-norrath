// Observed clone__4Gold at 0x115368, 368 bytes.
// Allocates 192 bytes; a null allocation returns null with no other call.
// Otherwise the GameObject vtable is stored at the new object+72 and the
// list-head copy constructor runs for offsets 0, 4 and 8 with the source's
// matching heads. Field copies follow, all from the source: bytes 12..27,
// 28..31, 32..35, 36..39, 40, 41, 44..59, 60..71, 76, 80..83, 84..87. The Gold
// vtable (_vt$4Gold, 0x44cd48) replaces the vtable word at offset 72 before the
// AnimationState copy constructor runs for offset 88. Then offset 128 (4 bytes)
// and offsets 132..191 are copied. Bytes 42, 43, 58, 59, 73..75 and 77..79 keep
// what the allocator left. The result is the new object.
// Unknown: the field meanings. Offsets 28, 32, 48, 60 and 64 are moved through
// float registers in the image; they are copied here as raw words.
#include "../inventory-items-object/object.hpp"

extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
void __14AnimationStateRC14AnimationState(void* destination, const void* source);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_player_view_44cd48[];
}

extern "C" void* clone__4Gold(void* source_storage) {
    using namespace inventory_object;
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(192);
    if (created == nullptr) return nullptr;
    write_word(created + 72, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_bytes(created + 12, source + 12, 4);
    copy_bytes(created + 16, source + 16, 12);
    copy_bytes(created + 28, source + 28, 4);
    copy_bytes(created + 36, source + 36, 4);
    copy_bytes(created + 32, source + 32, 4);
    copy_bytes(created + 40, source + 40, 2);
    write_word(created + 72, low_pointer(norrath_player_view_44cd48));
    copy_bytes(created + 48, source + 48, 4);
    copy_bytes(created + 44, source + 44, 4);
    copy_bytes(created + 52, source + 52, 4);
    copy_bytes(created + 60, source + 60, 4);
    copy_bytes(created + 56, source + 56, 2);
    copy_bytes(created + 64, source + 64, 8);
    copy_bytes(created + 76, source + 76, 1);
    copy_bytes(created + 80, source + 80, 8);
    __14AnimationStateRC14AnimationState(created + 88, source + 88);
    copy_bytes(created + 128, source + 128, 4);
    copy_bytes(created + 132, source + 132, 60);
    return created;
}
