// Observed: __16VIRealmInterfaceP20VIWndGenericRendererPCc at 0x01102ec8 is 864 bytes. With (this, renderer, name),
// word and byte offsets are in this:
//  1. The base object is built first: __Q212RealmService15RealmGatewayAPIUiPCcs(this, 2, name, -24735) where name
//     is the argument, or the string "con.soegames.net" (0x011d8da8) when the argument is null.
//  2. The word at +12 = renderer; the word at +8 = the address of _vt$16VIRealmInterface (0x01210298).
//  3. memset zeros, in this order: 256 bytes at +16; the word at +272 = 0; 1024 bytes at +276; 1024 at +1300; the
//     word at +2324 = 0; 20 bytes at +2328; 1024 at +2348; the word at +3372 = 0; 20 bytes at +3376;
//     __11VIUDPSocketi(this + 3396, -1); the words at +3468, +3472, +3476 = 0; 400 bytes at +3480; the word
//     at +3880 = 0; 400 bytes at +3884.
//  4. Two arrays of 100 strings, at +4284 and +4684 (one 32-bit data pointer each), are set to empty strings of the
//     old reference-counted library: the shared empty representation (0x011f6cb8, "nilRep") has its word at +12
//     tested; when that is nonzero the pointer is clone__Q2t12basic_string...Rep(nilRep), else it is nilRep + 16
//     and the representation's word at +8 (the reference count) is increased by one.
//  5. memset zero: 4576 bytes at +5088; the 64-bit value at +9664 = 0; 20 bytes at +9672; 24 bytes at +9692.
//  6. When the name argument (not the substituted default) is nonzero, printf("Using SRSServer (%s)\n", name).
//  7. The zeroing of step 3 and 5 is repeated with the same sizes in the order: +276 (1024), +1300 (1024), +2328
//     (20), +2348 (1024), +3376 (20), +16 (256), +3480 (400), +3884 (400), +5088 (4576), +9672 (20), +9692 (24).
//  8. The 64-bit value at +9728 = GetTime__7VITimer(); then zeros: 256 bytes at +9736, 25600 at +9992, 20 at +35592
//     (0x8b08), and a tail call of memset for 2400 bytes at +35612 (0x8b1c).
// Inferred: this builds the realm-service interface object: a gateway base, a UDP socket member, many fixed
// buffers, and two 100-entry string tables. The repeated zeroing is the sequence of a reset the image also has as
// a separate method (Reset__16VIRealmInterface, not compared here).
// Unknown: the roles of every buffer, the type of the renderer argument beyond a pointer, the exact meaning of
// the constants 2 and -24735 passed to the base, and the return type (the object pointer is not returned).
extern "C" {
struct StringRep {
    unsigned length;
    unsigned capacity;
    int references;
    int shared;
};
void __Q212RealmService15RealmGatewayAPIUiPCcs(void* self, unsigned a, const char* name, short b);
void __11VIUDPSocketi(void* self, int value);
void* memset(void* dest, int value, unsigned long size);
int printf(const char* format, ...);
unsigned long long GetTime__7VITimer();
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(StringRep* rep);
extern StringRep nilRep __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern unsigned char vt_realm_interface[] __asm__("_vt$16VIRealmInterface");

static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

static void fill_empty_strings(unsigned* slots) {
    for (int i = 0; i < 100; ++i) {
        unsigned word;
        if (nilRep.shared != 0) {
            word = word_of(clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(&nilRep));
        } else {
            word = word_of(&nilRep) + 16;
            nilRep.references = nilRep.references + 1;
        }
        slots[i] = word;
    }
}

void ctor_16VIRealmInterface(void* self, void* renderer, const char* name) __asm__("__16VIRealmInterfaceP20VIWndGenericRendererPCc");
void ctor_16VIRealmInterface(void* self, void* renderer, const char* name) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned* words = static_cast<unsigned*>(self);
    const char* use = name != 0 ? name : "con.soegames.net";
    __Q212RealmService15RealmGatewayAPIUiPCcs(self, 2, use, -24735);
    words[3] = word_of(renderer);
    words[2] = word_of(vt_realm_interface);
    memset(bytes + 16, 0, 256);
    words[68] = 0;
    memset(bytes + 276, 0, 1024);
    memset(bytes + 1300, 0, 1024);
    words[581] = 0;
    memset(bytes + 2328, 0, 20);
    memset(bytes + 2348, 0, 1024);
    words[843] = 0;
    memset(bytes + 3376, 0, 20);
    __11VIUDPSocketi(bytes + 3396, -1);
    words[867] = 0;
    words[868] = 0;
    words[869] = 0;
    memset(bytes + 3480, 0, 400);
    words[970] = 0;
    memset(bytes + 3884, 0, 400);
    fill_empty_strings(reinterpret_cast<unsigned*>(bytes + 4284));
    fill_empty_strings(reinterpret_cast<unsigned*>(bytes + 4684));
    memset(bytes + 5088, 0, 4576);
    *reinterpret_cast<unsigned long long*>(bytes + 9664) = 0;
    memset(bytes + 9672, 0, 20);
    memset(bytes + 9692, 0, 24);
    if (name != 0) {
        printf("Using SRSServer (%s)\n", name);
    }
    memset(bytes + 276, 0, 1024);
    memset(bytes + 1300, 0, 1024);
    memset(bytes + 2328, 0, 20);
    memset(bytes + 2348, 0, 1024);
    memset(bytes + 3376, 0, 20);
    memset(bytes + 16, 0, 256);
    memset(bytes + 3480, 0, 400);
    memset(bytes + 3884, 0, 400);
    memset(bytes + 5088, 0, 4576);
    memset(bytes + 9672, 0, 20);
    memset(bytes + 9692, 0, 24);
    *reinterpret_cast<unsigned long long*>(bytes + 9728) = GetTime__7VITimer();
    memset(bytes + 9736, 0, 256);
    memset(bytes + 9992, 0, 25600);
    memset(bytes + 0x8b08, 0, 20);
    memset(bytes + 0x8b1c, 0, 2400);
}
}
