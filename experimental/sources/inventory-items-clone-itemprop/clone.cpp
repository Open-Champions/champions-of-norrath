// Observed clone__8ItemProp at 0x1148f0, 672 bytes.
// Allocates 400 bytes; a null allocation returns null with no other call.
// Otherwise the GameObject vtable is stored at the new object+72, the list-head
// copy constructor runs for offsets 0, 4 and 8 with the source's matching
// heads, and the Prop vtable (_vt$4Prop, 0x44d018) is stored at offset 72
// while the field copies run. Copies from the source: 12..27, 28..31, 36..39,
// 32..35, 40, 41, 44..59, 60..71, 76..83, 84..99, 100..163, 164..227,
// 232..239, 240..251, 252..259, 260..271, 272..319. The ItemProp vtable
// (_vt$8ItemProp, 0x44cef8) is then stored at offset 72, offset 320..321 is
// copied, and 328..399 is copied last. Bytes 42, 43, 58, 59, 73..75, 228..231
// and 322..327 keep what the allocator left. No other call is made.
// Unknown: the field meanings. Several single-precision slots are copied as
// raw words.
#include "../inventory-items-object/object.hpp"

extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_player_view_44d018[];
extern const unsigned char norrath_player_view_44cef8[];
}

extern "C" void* clone__8ItemProp(void* source_storage) {
    using namespace inventory_object;
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(400);
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
    write_word(created + 72, low_pointer(norrath_player_view_44d018));
    copy_bytes(created + 48, source + 48, 4);
    copy_bytes(created + 44, source + 44, 4);
    copy_bytes(created + 52, source + 52, 4);
    copy_bytes(created + 60, source + 60, 4);
    copy_bytes(created + 56, source + 56, 2);
    copy_bytes(created + 68, source + 68, 4);
    copy_bytes(created + 64, source + 64, 4);
    copy_bytes(created + 76, source + 76, 8);
    copy_bytes(created + 84, source + 84, 16);
    copy_bytes(created + 100, source + 100, 64);
    copy_bytes(created + 164, source + 164, 64);
    copy_bytes(created + 232, source + 232, 8);
    copy_bytes(created + 240, source + 240, 12);
    copy_bytes(created + 252, source + 252, 8);
    copy_bytes(created + 260, source + 260, 12);
    copy_bytes(created + 272, source + 272, 48);
    write_word(created + 72, low_pointer(norrath_player_view_44cef8));
    copy_bytes(created + 320, source + 320, 2);
    copy_bytes(created + 328, source + 328, 72);
    return created;
}
