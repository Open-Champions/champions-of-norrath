// Observed _$_10GameObject at 0x141658, 680 bytes.
// Stores the GameObject vtable at object+72, then repeatedly selects the next
// "name=" argument. Each selection folds the tag at the existing tag-b view,
// compares four bytes, and requires '=' at byte 4. The first scan only tests
// that a value exists. The second scan passes its value, after the '=', to
// objectRemoveString. The tag fold and the argument reload are the same
// sequence already used by construction.
// Then listDestroy runs on object+8, object+4, and the object. Bit 0 of the
// flags word selects blockFree. Other flag bits do not.

extern "C" {
extern unsigned char norrath_constructor_tag_b_view[];
extern const unsigned char norrath_factory_ctype_minimum_view[];
extern const unsigned char norrath_game_object_vtable_view[];
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
void norrath_object_remove_string(char* text, void* object);
void norrath_list_destroy(void* head);
void norrath_block_free(void* object);
#if defined(NORRATH_GAMEOBJECT_HOST_CHECK)
extern unsigned long long norrath_gameobject_host_base;
#endif
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
int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}
unsigned char* from_word(unsigned word) {
#if defined(NORRATH_GAMEOBJECT_HOST_CHECK)
    return reinterpret_cast<unsigned char*>(norrath_gameobject_host_base + word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
unsigned to_word(const void* pointer) {
#if defined(NORRATH_GAMEOBJECT_HOST_CHECK)
    return static_cast<unsigned>(reinterpret_cast<unsigned long long>(pointer) - norrath_gameobject_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
bool lower_nonempty_tag(unsigned char* tag) {
    unsigned char raw = tag[0];
    if (raw == 0u) return false;
    do {
        const int character = signed_byte(raw);
        const unsigned flags = norrath_factory_ctype_minimum_view[unsigned(character + 128)];
        const int folded = (flags & 1u) != 0u ? character + 32 : character;
        *tag = static_cast<unsigned char>(folded);
        tag += 1;
        raw = tag[0];
    } while (raw != 0u);
    return true;
}
__attribute__((always_inline)) inline unsigned char* find_name_value(unsigned char* object, unsigned occurrence) {
    unsigned args_word = read_word(object + 44u);
    if (args_word == 0u) return nullptr;
    if (lower_nonempty_tag(norrath_constructor_tag_b_view)) args_word = read_word(object + 44u);
    unsigned char* args = from_word(args_word);
    if (read_word(args) == 0u) return nullptr;
    unsigned current = 0u;
    unsigned next = 0u;
    for (;;) {
        const unsigned entry_word = read_word(args + next);
        next += 4u;
        const int comparison = norrath_factory_strncmp(
            reinterpret_cast<char*>(from_word(entry_word)),
            reinterpret_cast<char*>(norrath_constructor_tag_b_view), 4u);
        args_word = read_word(object + 44u);
        args = from_word(args_word);
        if (comparison == 0) {
            const unsigned fresh = read_word(args + current);
            unsigned char* fresh_text = from_word(fresh);
            if (signed_byte(fresh_text[4]) == 61) {
                occurrence -= 1u;
                if (occurrence == 0xffffffffu) return fresh_text + 5;
            }
        }
        if (read_word(args + next) == 0u) return nullptr;
        current = next;
    }
}
}

extern "C" void norrath_vt_dtor_10GameObject(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, to_word(norrath_game_object_vtable_view));
    unsigned occurrence = 0u;
    for (;;) {
        unsigned char* present = find_name_value(object, occurrence);
        if (present == nullptr) break;
        unsigned char* value = find_name_value(object, occurrence);
        norrath_object_remove_string(reinterpret_cast<char*>(value), object);
        occurrence += 1u;
    }
    norrath_list_destroy(object + 8u);
    norrath_list_destroy(object + 4u);
    norrath_list_destroy(object);
    if ((flags & 1) != 0) norrath_block_free(object);
}
