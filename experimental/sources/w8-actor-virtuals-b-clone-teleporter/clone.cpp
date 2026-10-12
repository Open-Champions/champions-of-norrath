// Observed clone__10Teleporter at 0x001293a0, 1348 bytes.
// Allocates 960 bytes. A null allocation returns null and does not write.
// The GameObject vtable is stored at the new object+72 before the ListHead
// copy constructors at offsets 0, 4, and 8. Those calls zero the destination
// word and do not read the source. The Creature vtable then replaces it.
// Copied spans are the bytes the body stores from the source. Bytes 42, 43,
// 58, and 59 stay untouched, as do the one-byte and three-byte holes inside
// the later records. The AnimationState copy constructor owns object+288.
// ListHead copy constructors then run at 576, 752, and 888.
// The Creature vtable (view 0x4488f0) is the one the animation copy and the later
// list copies observe. After the last copied byte (959) object+72 is replaced with
// the Teleporter vtable (view 0x448e30). Apart from that final store and the
// symbol, the copy sequence is the same as the one in clone__8Creature.

extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
void animation_state_copy(void* destination, const void* source) asm("__15AnimationState8RC15AnimationState8");
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_actor_view_4488f0[];
extern const unsigned char norrath_actors_b_view_448e30[];
}

namespace {
struct Span { unsigned offset; unsigned count; };
unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
void copy_spans(unsigned char* dest, const unsigned char* source, const Span* spans, unsigned count) {
    for (unsigned s = 0; s < count; ++s) {
        for (unsigned i = 0; i < spans[s].count; ++i) dest[spans[s].offset + i] = source[spans[s].offset + i];
    }
}
const Span before_animation[] = { {12, 30}, {44, 14}, {60, 12}, {76, 1}, {80, 6}, {88, 69}, {160, 14}, {176, 14}, {192, 14}, {208, 26}, {236, 50} };
const Span before_list_576[] = { {552, 19}, {572, 2} };
const Span before_list_752[] = { {580, 170} };
const Span before_list_888[] = { {756, 45}, {804, 84} };
const Span tail[] = { {892, 68} };
}

extern "C" void* clone__10Teleporter(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(960);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_spans(created, source, before_animation, sizeof before_animation / sizeof before_animation[0]);
    write_word(created + 72u, low_pointer(norrath_actor_view_4488f0));
    animation_state_copy(created + 288, source + 288);
    copy_spans(created, source, before_list_576, sizeof before_list_576 / sizeof before_list_576[0]);
    norrath_list_head_copy_construct(created + 576, source + 576);
    copy_spans(created, source, before_list_752, sizeof before_list_752 / sizeof before_list_752[0]);
    norrath_list_head_copy_construct(created + 752, source + 752);
    copy_spans(created, source, before_list_888, sizeof before_list_888 / sizeof before_list_888[0]);
    norrath_list_head_copy_construct(created + 888, source + 888);
    copy_spans(created, source, tail, sizeof tail / sizeof tail[0]);
    write_word(created + 72u, low_pointer(norrath_actors_b_view_448e30));
    return created;
}
