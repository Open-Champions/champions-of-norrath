// Observed: __Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI at 0x01119e28, 256 bytes, pinned
// SLUS_205.65, a constructor of a 144-byte object. With (this, name, b, api), b a 16-bit value the routine sign-extends itself
// from the low halfword of its argument register (declared int here and narrowed in the body):
//  1. The generic API base is built: __Q210GenericAPI14GenericAPICorePCcsUiUiUiUiUiUiUiUi(this, name, b, 60, 5, 0, 90, 32, 32,
//     120, 64) (the last three of the eight unsigned arguments go on the stack).
//  2. The word at +104 = api, +108 = 0, +100 = the address of _vt$Q212RealmService19RealmGatewayAPICore, +112 = 0.
//  3. __9Encryptor(this + 116) constructs the member at +116.
//  4. The old reference-counted string at +132 is set to an empty string: when the word at +12 of the shared empty
//     representation (nilRep, 0x011f6cb8) is nonzero the pointer is clone__Q2t12basic_string...Rep(nilRep), else it is
//     nilRep + 16 and the representation's word at +8 (the reference count) is increased by one.
//  5. replace__t12basic_string...UiUiPCcUi(this + 132, 0, -1, name, strlen(name)): the string becomes a copy of name.
//  6. The halfword at +136 = b.
// Nothing is returned.
// Inferred: this is a gateway API client core: a generic API base, a back pointer to the wrapper at +104, an encryptor
// member, a name string and a 16-bit value (the wrapper passes -24735 for it, in the caller that builds the realm interface).
// Unknown: the meaning of the eight generic-base constants, of the words at +108 and +112, and of the 16-bit value.
extern "C" {
struct StringRep {
    unsigned length;
    unsigned capacity;
    int references;
    int shared;
};
extern StringRep nilRep __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern unsigned char vt_realm_gateway_api_core[] __asm__("_vt$Q212RealmService19RealmGatewayAPICore");

unsigned strlen(const char* text);
void __Q210GenericAPI14GenericAPICorePCcsUiUiUiUiUiUiUiUi(void* self, const char* name, short b, unsigned first, unsigned second,
                                                          unsigned third, unsigned fourth, unsigned fifth, unsigned sixth,
                                                          unsigned seventh, unsigned eighth);
void __9Encryptor(void* self);
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(StringRep* rep);
void replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(
    void* string, unsigned position, unsigned count, const char* text, unsigned length);

static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void __Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI(void* self, const char* name, int b_word,
                                                                                    void* api) {
    const short b = static_cast<short>(b_word);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned* words = static_cast<unsigned*>(self);
    __Q210GenericAPI14GenericAPICorePCcsUiUiUiUiUiUiUiUi(self, name, b, 60, 5, 0, 90, 32, 32, 120, 64);
    words[26] = word_of(api);
    words[27] = 0;
    words[25] = word_of(vt_realm_gateway_api_core);
    words[28] = 0;
    __9Encryptor(bytes + 116);
    unsigned empty;
    if (nilRep.shared != 0) {
        empty = word_of(clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(&nilRep));
    } else {
        empty = word_of(&nilRep) + 16;
        nilRep.references = nilRep.references + 1;
    }
    words[33] = empty;
    replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(bytes + 132, 0, 0xffffffffu, name,
                                                                                                    strlen(name));
    *reinterpret_cast<short*>(bytes + 136) = b;
}
}
