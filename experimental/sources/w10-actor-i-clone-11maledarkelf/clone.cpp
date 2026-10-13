// Observed clone__11MaleDarkElf at 0x0011c8c8, 1580 bytes.
// Allocates 1120 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// The GameObject table pointer is stored at the new object+72 first, then the ListHead copy
// constructor runs for the heads at +0, +4 and +8 (it zeroes the destination word and does not read the
// source). Source spans are copied in the groups below, in the image's order; the Creature table
// pointer replaces the word at +72 before the AnimationState copy constructor runs on the member at
// +288, ListHead copies follow at +576, +752 and +888, and the MaleDarkElf table pointer (norrath_ctors_lava_wraith_view_450440) replaces the
// word at +72 last. Bytes outside the spans keep what the allocator returned.
// Copy spans were measured by running the original routine on a pattern-filled source. Inferred:
// the member at +288 is an animation state and the heads at +576, +752 and +888 are lists, from the
// callees' parameter types. Unknown: the meaning of the other fields. Nothing here ran on the EE.
extern "C" {
unsigned char* blockAlloc__Fi(int requested_size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void animation_state_copy(void* destination, const void* source) __asm__("__15AnimationState8RC15AnimationState8");
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_actor_view_4488f0[];
extern const unsigned char norrath_ctors_lava_wraith_view_450440[];
}

namespace {
struct Span { unsigned offset; unsigned count; };
unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    *reinterpret_cast<unsigned*>(p) = value;
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
const Span tail[] = { {892, 69}, {964, 72}, {1040, 76} };
}

extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__11MaleDarkElf(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = blockAlloc__Fi(1120);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    ListHead_copy(created, source);
    ListHead_copy(created + 4, source + 4);
    ListHead_copy(created + 8, source + 8);
    copy_spans(created, source, before_animation, sizeof before_animation / sizeof before_animation[0]);
    write_word(created + 72u, low_pointer(norrath_actor_view_4488f0));
    animation_state_copy(created + 288, source + 288);
    copy_spans(created, source, before_list_576, sizeof before_list_576 / sizeof before_list_576[0]);
    ListHead_copy(created + 576, source + 576);
    copy_spans(created, source, before_list_752, sizeof before_list_752 / sizeof before_list_752[0]);
    ListHead_copy(created + 752, source + 752);
    copy_spans(created, source, before_list_888, sizeof before_list_888 / sizeof before_list_888[0]);
    ListHead_copy(created + 888, source + 888);
    copy_spans(created, source, tail, sizeof tail / sizeof tail[0]);
    write_word(created + 72u, low_pointer(norrath_ctors_lava_wraith_view_450440));
    return created;
}
