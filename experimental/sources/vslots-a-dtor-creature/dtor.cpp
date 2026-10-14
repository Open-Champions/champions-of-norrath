// Observed _$_8Creature at 0x00112340, 244 bytes.
// Stores the Creature vtable at object+72, then walks the list at object+888
// with the safe iterator. Each node word at +12 is an object whose vtable word
// is at +20. The slot 8 bytes into that vtable is called with argument 3.
// A zero object word skips the call. The iterator is then destroyed with flag 2.
// listDestroy follows on object+888, object+752, and object+576. The
// FxCreatureState destructor runs at object+824 with flag 2, and the
// AnimationState destructor at object+288 with flag 0. GameObject's destructor
// runs with flag 0. Incoming flag bit 0 selects blockFree. Other flag bits do not.

#if defined(NORRATH_VSLOTS_A_HOST)
extern unsigned long long norrath_vslots_a_host_base;
#endif

extern "C" {
extern const unsigned char norrath_actor_view_4488f0[];
void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask);
void norrath_list_safe_iterator_next(unsigned char* iterator);
void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags);
void norrath_list_destroy(void* head);
void fx_creature_state_destructor(void* state, int flags) asm("_$_15FxCreatureState");
void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int read_half(const unsigned char* p) {
    unsigned value = unsigned(p[0]) | (unsigned(p[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 0x10000;
}
unsigned to_word(const void* pointer) {
#if defined(NORRATH_VSLOTS_A_HOST)
    return static_cast<unsigned>(reinterpret_cast<unsigned long long>(pointer) - norrath_vslots_a_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<unsigned long long>(pointer));
#endif
}
unsigned char* from_word(unsigned word) {
#if defined(NORRATH_VSLOTS_A_HOST)
    return reinterpret_cast<unsigned char*>(norrath_vslots_a_host_base + static_cast<long long>(static_cast<int>(word)));
#else
    return reinterpret_cast<unsigned char*>(static_cast<unsigned long long>(word));
#endif
}
}

extern "C" void creature_destructor(void* self, int flags) asm("_$_8Creature");
extern "C" void creature_destructor(void* self, int flags) {
    auto* object = static_cast<unsigned char*>(self);
    write_word(object + 72, to_word(norrath_actor_view_4488f0));
    unsigned char iterator[24];
    norrath_list_safe_iterator_construct(iterator, object + 888, 0);
    unsigned node_word = read_word(iterator + 4);
    while (node_word != 0u) {
        unsigned char* node = from_word(node_word);
        unsigned child_word = read_word(node + 12);
        if (child_word != 0u) {
            unsigned char* child = from_word(child_word);
            unsigned char* slot = from_word(read_word(child + 20)) + 8;
            const int adjustment = read_half(slot);
            using SlotCall = void (*)(void*, int);
            auto* call = reinterpret_cast<SlotCall>(from_word(read_word(slot + 4)));
            call(child + adjustment, 3);
        }
        norrath_list_safe_iterator_next(iterator);
        node_word = read_word(iterator + 4);
    }
    norrath_list_safe_iterator_destroy(iterator, 2);
    norrath_list_destroy(object + 888);
    fx_creature_state_destructor(object + 824, 2);
    norrath_list_destroy(object + 752);
    norrath_list_destroy(object + 576);
    animation_state_destructor(object + 288, 0);
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
