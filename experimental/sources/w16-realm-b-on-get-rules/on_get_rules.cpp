#include "../w9-animation-object-c-common/mem.hpp"
// Observed onGetRules__16VIRealmInterfaceUiiPCUsUiPv at 0x01108c60, 928 bytes. The image reserves a 614,560-byte frame: a 614,400-byte
// buffer for the formatted text, then (from 0x96030 above the frame base) three string handles. With (this, a, status, text, length, extra):
//  1. printf("VIRealmInterface::onGetRules(%i, %i)\n", a, status); app = GetAppPtr__5VIWnd(the word at this+12).
//  2. snprintf16__8VIStringPUsiPCce(buffer, 0x96000, "%S\n\n\n\n", text); ReceiveEula__9VIStationiPCUsi(app, status, buffer, length + 4).
//  3. The image's inlined string library is then used for a diagnostic print: a wide string is built from the shared empty string
//     (a copy through clone__...3Rep when the empty rep's word at +12 is nonzero, otherwise its count at +8 is incremented) and replaced
//     with the (text, length) pair; a narrow string is built the same way and resized to the wide length; each wide unit is narrowed
//     by keeping its low byte (before each store, when the narrow rep's count at +8 is at least 2 it is made private with alloc(...,
//     length, true), and the word before the characters, the shared flag, is set to 1); the narrow string is then shared into a copy
//     (clone when the flag word at +12 is nonzero, else its count is incremented), the narrow string released, the terminator written
//     at index length when the length is nonzero, and printf("%s\n", copy or "" when the length is zero) is called; finally the copy
//     and the wide string are released.
//  A handle is a pointer to the characters; the 16-byte header before it holds length (+0), capacity (+4), count (+8), flag (+12).
//  Release drops the count; at zero the block goes on the pool free list indexed by ((capacity * element + 16 + 7) / 8) - 1 when
//  the size is at most 128 bytes, else __wrap_free.
// Inferred: this is the reply to a request for the realm's rules/EULA text, handed to the station object for display, and echoed to
// the log in narrowed form. The extra argument is never read. The handling of the string library mirrors OnKeyDown's.
// Unknown: what the length + 4 passed to ReceiveEula stands for; whether the 614,400 passed as the count is a byte or a unit count; whether status is a status
// or an id (it is both printed and passed on).
extern "C" {
extern unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
extern unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

int printf(const char* format, ...);
void* GetAppPtr__5VIWnd(void* renderer);
int snprintf16__8VIStringPUsiPCce(unsigned short* out, int count, const char* format, ...);
void ReceiveEula__9VIStationiPCUsi(void* station, int status, const unsigned short* text, int length);
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(void* rep);
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep);
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(
    void* string, unsigned position, unsigned count, const unsigned short* source, unsigned length);
void resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(void* string, unsigned length, char fill);
void alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(void* string, unsigned length, bool flag);
void __wrap_free(void* block);

static inline unsigned word_at(const void* base, int offset) {
    return *reinterpret_cast<const unsigned*>(static_cast<const unsigned char*>(base) + offset);
}
// Drops one reference to the string whose characters start at *handle; element is the character size in bytes.
static inline void release(unsigned char** handle, unsigned element) {
    unsigned char* rep = *handle - 16;
    unsigned count = *reinterpret_cast<unsigned*>(rep + 8) - 1;
    *reinterpret_cast<unsigned*>(rep + 8) = count;
    if (count != 0) return;
    unsigned size = *reinterpret_cast<unsigned*>(rep + 4) * element + 16;
    if (size < 129) {
        unsigned* slot = &norrath_free_list[((size + 7) >> 3) - 1];
        *reinterpret_cast<unsigned*>(rep) = *slot;
        norrath_aoc::store_pointer(reinterpret_cast<unsigned char*>(slot), rep);
    } else {
        __wrap_free(rep);
    }
}

void onGetRules__16VIRealmInterfaceUiiPCUsUiPv(void* self, unsigned a, int status, const unsigned short* text, unsigned length, void* extra) {
    (void)extra;
    unsigned short buffer[307200];
    printf("VIRealmInterface::onGetRules(%i, %i)\n", a, status);
    void* app = GetAppPtr__5VIWnd(norrath_aoc::load_pointer(static_cast<unsigned char*>(self) + 12));
    snprintf16__8VIStringPUsiPCce(buffer, 0x96000, "%S\n\n\n\n", text);
    ReceiveEula__9VIStationiPCUsi(app, status, buffer, static_cast<int>(length + 4));
    unsigned char* wide;
    if (word_at(norrath_nil_rep_ushort, 12) != 0) {
        wide = clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(norrath_nil_rep_ushort);
    } else {
        *reinterpret_cast<unsigned*>(norrath_nil_rep_ushort + 8) = word_at(norrath_nil_rep_ushort, 8) + 1;
        wide = norrath_nil_rep_ushort + 16;
    }
    replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(&wide, 0, 0xffffffffu, text, length);
    unsigned char* narrow;
    if (word_at(norrath_nil_rep_char, 12) != 0) {
        narrow = clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(norrath_nil_rep_char);
    } else {
        *reinterpret_cast<unsigned*>(norrath_nil_rep_char + 8) = word_at(norrath_nil_rep_char, 8) + 1;
        narrow = norrath_nil_rep_char + 16;
    }
    resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(&narrow, word_at(wide - 16, 0), 0);
    {
        const unsigned short* from = reinterpret_cast<const unsigned short*>(wide);
        const unsigned short* end = from + word_at(wide - 16, 0);
        unsigned index = 0;
        while (from != end) {
            if (word_at(narrow - 16, 8) >= 2)
                alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(&narrow, word_at(narrow - 16, 0), true);
            const unsigned char low = static_cast<unsigned char>(*from);
            from += 1;
            *reinterpret_cast<unsigned*>(narrow - 4) = 1;
            narrow[index] = low;
            index += 1;
        }
    }
    unsigned char* copy;
    if (word_at(narrow - 16, 12) != 0) {
        copy = clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(narrow - 16);
    } else {
        *reinterpret_cast<unsigned*>(narrow - 8) = word_at(narrow - 16, 8) + 1;
        copy = narrow;
    }
    release(&narrow, 1);
    const char* shown = "";
    if (word_at(copy - 16, 0) != 0) {
        copy[word_at(copy - 16, 0)] = 0;
        shown = reinterpret_cast<const char*>(copy);
    }
    printf("%s\n", shown);
    release(&copy, 1);
    release(&wide, 2);
}
}
