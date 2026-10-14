// Observed createPrivateGame__16VIRealmInterfaceiPCUsN22Pv at 0x01103c50, 848 bytes, 144-byte frame. With (this, count,
// first, second, third, context), the three strings being 16-bit-character strings ending in a zero character:
//  1. The word at this + 3472 = count.
//  2. wide = an empty string of the old reference-counted library for 16-bit characters (the shared empty
//     representation, see below); length = the number of characters of second before its terminator;
//     replace__t12basic_string...ZUs...(&wide, 0, -1, second, length) fills it, so wide holds a copy of second.
//  3. narrow = an empty 8-bit string; resize__t12basic_string...Zc...(&narrow, length of wide, 0). Then for each
//     16-bit character of wide in order: when the representation of narrow has a reference count of two or more
//     (unsigned test) alloc__t12basic_string...Zc...(&narrow, its length, true); the representation's flag word is
//     set to 1; the character's low byte is stored at narrow[i].
//  4. copy = a copy of narrow: when the flag word of narrow's representation is nonzero, clone__...Rep(rep) gives
//     the copy, else the reference count is increased and the data pointer is shared. Then narrow is released.
//  5. text = copy's data, with a zero byte stored after the last character, or the empty literal when the length
//     is zero; snprintf(this + 276, 1023, text) -- the narrowed string itself is the format string. copy is released.
//  6. createPrivateGame__Q212RealmService15RealmGatewayAPIPCUsN21Pv(this, first, second, third, context), then wide
//     is released.
// A string is one word, the pointer to its data; its representation is 16 bytes before it: a length word, a
// capacity word, a reference count and a flag word. Release: the reference count falls by one and at zero a block of
// capacity * (character size) + 16 bytes goes back to the 16-entry free list
// _t24__default_alloc_template2b0i0$_S_free_list (index ((bytes + 23) >> 3) - 1, when bytes is below 129) or to
// __wrap_free. The same layout is read by the realm-interface constructor and destructor sources.
// Inferred: the 16-bit strings are text typed by the player (names), narrowed to bytes for the buffer at +276; the
// narrowing keeps only the low byte of each character.
// Unknown: the roles of first, third and context in the base call, what +3472 holds, and
// whether the format-string use of the text is intended (it is what the image does).
extern "C" {
struct StringRep {
    unsigned length;
    unsigned capacity;
    int references;
    int shared;
};
int snprintf(char* dest, unsigned long size, const char* format, ...);
void __wrap_free(void* pointer);
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(
    void* self, unsigned position, unsigned count, const unsigned short* text, unsigned length);
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(StringRep* rep);
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(StringRep* rep);
void resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(void* self, unsigned length, char fill);
void alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(void* self, unsigned length, bool flag);
void createPrivateGame__Q212RealmService15RealmGatewayAPIPCUsN21Pv(void* self, const unsigned short* first,
                                                                   const unsigned short* second,
                                                                   const unsigned short* third, void* context);
extern StringRep nilRepWide __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
extern StringRep nilRepNarrow __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern unsigned free_list[] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");
}

namespace {
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned norrath_host_word_of(const void* pointer);
extern "C" void* norrath_host_pointer_of(unsigned word);
inline unsigned word_of(const void* pointer) { return norrath_host_word_of(pointer); }
inline void* pointer_of(unsigned word) { return norrath_host_pointer_of(word); }
#else
inline unsigned word_of(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
inline void* pointer_of(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
#endif
inline StringRep* rep_of(unsigned data) { return static_cast<StringRep*>(pointer_of(data - 16)); }

unsigned empty_wide() {
    if (nilRepWide.shared != 0)
        return word_of(clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(&nilRepWide));
    nilRepWide.references = nilRepWide.references + 1;
    return word_of(&nilRepWide) + 16;
}
unsigned empty_narrow() {
    if (nilRepNarrow.shared != 0)
        return word_of(clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(&nilRepNarrow));
    nilRepNarrow.references = nilRepNarrow.references + 1;
    return word_of(&nilRepNarrow) + 16;
}
unsigned copy_of_narrow(unsigned data) {
    StringRep* rep = rep_of(data);
    if (rep->shared != 0)
        return word_of(clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(rep));
    rep->references = rep->references + 1;
    return data;
}
void release(unsigned data, unsigned character_size) {
    StringRep* rep = rep_of(data);
    rep->references = rep->references - 1;
    if (rep->references == 0) {
        const unsigned bytes = rep->capacity * character_size;
        if (bytes + 16 < 129) {
            const unsigned index = ((bytes + 23) >> 3) - 1;
            *reinterpret_cast<unsigned*>(rep) = free_list[index];
            free_list[index] = word_of(rep);
        } else {
            __wrap_free(rep);
        }
    }
}
}

extern "C" void createPrivateGame__16VIRealmInterfaceiPCUsN22Pv(void* self, int count, const unsigned short* first,
                                                                const unsigned short* second,
                                                                const unsigned short* third, void* context) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<int*>(bytes + 3472) = count;
    unsigned wide = empty_wide();
    unsigned length = 0;
    while (second[length] != 0) ++length;
    replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(&wide, 0, 0xffffffffu,
                                                                                                       second, length);
    unsigned narrow = empty_narrow();
    resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(&narrow, rep_of(wide)->length, 0);
    const unsigned char* source = static_cast<const unsigned char*>(pointer_of(wide));
    const unsigned char* end = source + rep_of(wide)->length * 2;
    for (unsigned i = 0; source != end; source += 2, ++i) {
        if (static_cast<unsigned>(rep_of(narrow)->references) >= 2)
            alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(&narrow, rep_of(narrow)->length, true);
        rep_of(narrow)->shared = 1;
        static_cast<unsigned char*>(pointer_of(narrow))[i] = *source;
    }
    unsigned copy = copy_of_narrow(narrow);
    release(narrow, 1);
    const char* text;
    if (rep_of(copy)->length == 0) {
        text = "";
    } else {
        static_cast<char*>(pointer_of(copy))[rep_of(copy)->length] = 0;
        text = static_cast<const char*>(pointer_of(copy));
    }
    snprintf(reinterpret_cast<char*>(bytes + 276), 1023, text);
    release(copy, 1);
    createPrivateGame__Q212RealmService15RealmGatewayAPIPCUsN21Pv(self, first, second, third, context);
    release(wide, 2);
}
