// Observed onConnect__16VIRealmInterface at 0x01103fa0, 2056 bytes, 256-byte frame. With (this):
//  1. The word `connected` = 1; printf("Connected to realm interface\n").
//  2. user = the 16-bit string for the 3-character literal at 0x011d8e60, built as below; password likewise from
//     the literal at 0x011d8e68. Both literals are short test-like words (see "Unknown").
//  3. login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(this, user, password, 0), then password and user are released.
// Building a 16-bit string from a literal (all with the old reference-counted library; a string is one word, the
// data pointer, 16 bytes after its representation: length, capacity, reference count, flag word; the same layout
// as the realm-interface createPrivateGame source):
//  a. narrow = an empty 8-bit string; replace__...Zc...(&narrow, 0, -1, literal, strlen(literal)).
//  b. wide0 = an empty 16-bit string; resize__...ZUs...(&wide0, length of narrow, 0). For each byte of narrow in
//     order: when wide0's representation has a reference count of two or more (unsigned test),
//     alloc__...ZUs...(&wide0, its length, true); the flag word is set to 1; wide0[i] = the byte sign-extended to 16 bits.
//  c. wide1 = a copy of wide0 (flag word nonzero: clone__...Rep, else share and count up); release wide0.
//  d. wide2 = a copy of wide1; replace__...ZUs...UiUiUiUs(&wide2, length, 0, 1, 0) appends one zero character;
//     result = a copy of wide2; release wide2, then wide1, then narrow.
// Block 1 gives user, block 2 gives password; after the login call password is released, then user.
// Release: the reference count falls by one; at zero a block of capacity * character size + 16 bytes returns to
// _t24__default_alloc_template2b0i0$_S_free_list (index ((bytes + 23) >> 3) - 1 when bytes is below 129) or to __wrap_free.
// Inferred: the routine logs into the realm service with a fixed pair of 16-bit credential strings after the
// connection came up; the chain of copies is the shape of by-value returns of the library, not separate design.
// Unknown: whether the two literals are placeholders; what the zero-terminator append is for; the return type
// (void: nothing is returned).
extern "C" {
struct StringRep {
    unsigned length;
    unsigned capacity;
    int references;
    int shared;
};
int printf(const char* format, ...);
unsigned long strlen(const char* text);
void __wrap_free(void* pointer);
void login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(void* self, const unsigned short* user, const unsigned short* password,
                                                      void* context);
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(StringRep* rep);
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(StringRep* rep);
void replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(
    void* self, unsigned position, unsigned count, const char* text, unsigned length);
void resize__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUs(void* self, unsigned length,
                                                                                               unsigned short fill);
void alloc__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0Uib(void* self, unsigned length,
                                                                                              bool flag);
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiUiUs(
    void* self, unsigned position, unsigned removed, unsigned count, unsigned short character);
extern int connected;
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
unsigned copy_of_wide(unsigned data) {
    StringRep* rep = rep_of(data);
    if (rep->shared != 0)
        return word_of(clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(rep));
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

// Steps a to d; the result is the final copy (one reference more than its data holds when shared).
unsigned wide_string_from_literal(const char* literal) {
    unsigned narrow = empty_narrow();
    const unsigned long literal_length = strlen(literal);
    replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(&narrow, 0, 0xffffffffu,
                                                                                                    literal, literal_length);
    unsigned wide0 = empty_wide();
    resize__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUs(&wide0, rep_of(narrow)->length, 0);
    const signed char* source = static_cast<const signed char*>(pointer_of(narrow));
    const signed char* end = source + rep_of(narrow)->length;
    for (unsigned i = 0; source != end; ++source, ++i) {
        if (static_cast<unsigned>(rep_of(wide0)->references) >= 2)
            alloc__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0Uib(&wide0, rep_of(wide0)->length, true);
        rep_of(wide0)->shared = 1;
        static_cast<short*>(pointer_of(wide0))[i] = *source;
    }
    unsigned wide1 = copy_of_wide(wide0);
    release(wide0, 2);
    unsigned wide2 = copy_of_wide(wide1);
    replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiUiUs(&wide2, rep_of(wide2)->length,
                                                                                                      0, 1, 0);
    unsigned result = copy_of_wide(wide2);
    release(wide2, 2);
    release(wide1, 2);
    release(narrow, 1);
    return result;
}
}

extern "C" void onConnect__16VIRealmInterface(void* self) {
    connected = 1;
    printf("Connected to realm interface\n");
    unsigned user = wide_string_from_literal("foo");
    unsigned password = wide_string_from_literal("bar");
    login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(self, static_cast<const unsigned short*>(pointer_of(user)),
                                                     static_cast<const unsigned short*>(pointer_of(password)), nullptr);
    release(password, 2);
    release(user, 2);
}
