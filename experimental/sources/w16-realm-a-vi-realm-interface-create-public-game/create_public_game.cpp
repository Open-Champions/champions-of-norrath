// Observed createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv at 0x011038d0,
// 896 bytes, 176-byte frame. With (this, count, first, number, a, b, c, text, attributes, last, context), the strings being
// 16-bit-character strings ending in a zero character, `attributes` a reference (passed as a pointer) and `number` a
// 16-bit value that the routine sign-extends itself (the argument register is not trusted; the source takes an int and
// narrows). The last two parameters arrive in stack slots 1 and 2 of the caller's argument area, the reference in slot 0:
//  1. The word at this + 3472 = count.
//  2. wide = an empty 16-bit string (the shared empty representation); length = the number of characters of text before
//     its terminator; replace__t12basic_string...ZUs...(&wide, 0, -1, text, length) makes wide a copy of text.
//  3. narrow = an empty 8-bit string; resize__t12basic_string...Zc...(&narrow, length of wide, 0). For each 16-bit
//     character of wide in order: when narrow's representation has a reference count of two or more (unsigned
//     test), alloc__t12basic_string...Zc...(&narrow, its length, true); the flag word is set to 1; the low byte of
//     the character is stored at narrow[i].
//  4. copy = a copy of narrow (flag word nonzero: clone__...Rep, else share and count up); narrow is released.
//  5. snprintf(this + 276, 1023, c_str of copy) -- the narrowed text is the format string; the empty literal when the
//     length is zero, else a zero byte is stored after the last character. copy is released.
//  6. createPublicGame__Q212RealmService15RealmGatewayAPIPCUssUiUiUiT1RCQ212RealmService14MatchGameAttrsT1Pv(this,
//     first, (short) number, a, b, c, text, attributes, last, context); then wide is released.
// The string layout and release rule are those of the createPrivateGame source of this package: a string is the data
// pointer; the representation (length, capacity, reference count, flag word) is 16 bytes before it; a released block
// of capacity * character size + 16 bytes below 129 goes to _t24__default_alloc_template2b0i0$_S_free_list at index
// ((bytes + 23) >> 3) - 1, else to __wrap_free.
// Inferred: the public counterpart of createPrivateGame: the same text-to-buffer step (the buffer at +276 receives the
// narrowed `text`), then a call to the gateway with the match attributes.
// Unknown: the roles of every numeric argument (number, a, b, c), of first and last, and of the buffer's later users;
// the meaning of +3472; whether the format-string use of the text is intended (it is what the image does).
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
void createPublicGame__Q212RealmService15RealmGatewayAPIPCUssUiUiUiT1RCQ212RealmService14MatchGameAttrsT1Pv(
    void* self, const unsigned short* first, short number, unsigned a, unsigned b, unsigned c, const unsigned short* text,
    const void* attributes, const unsigned short* last, void* context);
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

extern "C" void createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv(
    void* self, int count, const unsigned short* first, int number, unsigned a, unsigned b, unsigned c,
    const unsigned short* text, const void* attributes, const unsigned short* last, void* context) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    const short short_number = static_cast<short>(number);
    *reinterpret_cast<int*>(bytes + 3472) = count;
    unsigned wide = empty_wide();
    unsigned length = 0;
    while (text[length] != 0) ++length;
    replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(&wide, 0, 0xffffffffu,
                                                                                                       text, length);
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
    const char* format;
    if (rep_of(copy)->length == 0) {
        format = "";
    } else {
        static_cast<char*>(pointer_of(copy))[rep_of(copy)->length] = 0;
        format = static_cast<const char*>(pointer_of(copy));
    }
    snprintf(reinterpret_cast<char*>(bytes + 276), 1023, format);
    release(copy, 1);
    createPublicGame__Q212RealmService15RealmGatewayAPIPCUssUiUiUiT1RCQ212RealmService14MatchGameAttrsT1Pv(
        self, first, short_number, a, b, c, text, attributes, last, context);
    release(wide, 2);
}
