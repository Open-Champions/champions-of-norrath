#include "../w9-animation-object-c-common/mem.hpp"
// Observed onLogin__16VIRealmInterfaceUiiPCUsPv at 0x011068d0, 2132 bytes (a 256-byte frame; six string handles live in it). With
// (this, a, status, name, extra), where extra is never read; offsets are in this:
//  1. GetAppPtr__5VIWnd(the word at this+12) (result unused); UnLock__11VIGUIDialog(the word at (the word at this+12) + 56).
//  2. status == 0: strncpyWide__FPUsPCUsUi(this+16, name, 256); the word at +272 = strlenWide__FPCUs(this+16).
//  3. status != 0: two wide strings are built from the literals "foo" and "bar" through the image's inlined reference-counted string
//     code, and login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(this, first, second, 0) is called with them. Each wide string is built as:
//       narrow = the shared empty narrow string (a clone when its rep's word at +12 is nonzero, else its count at +8 is incremented),
//       replaced with (literal, strlen(literal)); wide = the shared empty wide string the same way, then resized to the narrow length
//       with fill 0; each narrow byte is stored as a 16-bit unit sign-extended from the byte (before each store, if the wide rep's
//       count at +8 is at least 2 it is made private with alloc(..., length, true); the word before the characters, the flag, is
//       set to 1); then the string is shared into a copy (clone when the flag is nonzero, else the count is incremented), the
//       original wide string is released, the copy is shared into a second copy to which one zero unit is appended with
//       replace(second, length, 0, 1, 0), that is shared into a third copy, and finally the second copy, the first copy and the
//       narrow string are released. The third copy is what login receives. After login the second string and then the first are released.
//  4. In all cases printf("VIRealmInterface::onLogin(%i, %i)\n", a, status) ends the call.
//  A handle is a pointer to the characters; the 16-byte header before it holds length (+0), capacity (+4), count (+8), flag (+12). Release
//  drops the count; at zero the block goes on the pool free list indexed by ((capacity * element + 16 + 7) / 8) - 1 when its size is at most
//  128 bytes, else __wrap_free.
// Inferred: the reply to a connect/authorize step; with a nonzero status the interface logs in with placeholder credentials "foo"/"bar"
// (a debugging or test path). The name copied for status 0 is kept at +16 with its length at +272.
// Unknown: the meaning of a and status values; why the placeholder login exists; the frame's unused space.
extern "C" {
extern unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
extern unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

int printf(const char* format, ...);
unsigned long strlen(const char* text);
void* GetAppPtr__5VIWnd(void* renderer);
void UnLock__11VIGUIDialog(void* dialog);
void strncpyWide__FPUsPCUsUi(unsigned short* out, const unsigned short* in, unsigned count);
int strlenWide__FPCUs(const unsigned short* text);
void login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(void* api, const unsigned short* first, const unsigned short* second, void* data);
void __wrap_free(void* block);
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(void* rep);
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep);
void replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(
    void* string, unsigned position, unsigned count, const char* source, unsigned length);
void resize__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUs(void* string, unsigned length, unsigned short fill);
void alloc__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0Uib(void* string, unsigned length, bool flag);
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiUiUs(
    void* string, unsigned position, unsigned count, unsigned repeat, unsigned short unit);

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
static inline unsigned char* share_wide(unsigned char* handle) {
    if (word_at(handle - 16, 12) != 0)
        return clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(handle - 16);
    *reinterpret_cast<unsigned*>(handle - 8) = word_at(handle - 16, 8) + 1;
    return handle;
}
// Builds the wide string that login receives for one literal (see the notes above).
static unsigned char* wide_login_text(const char* literal) {
    unsigned char* narrow;
    if (word_at(norrath_nil_rep_char, 12) != 0) {
        narrow = clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(norrath_nil_rep_char);
    } else {
        *reinterpret_cast<unsigned*>(norrath_nil_rep_char + 8) = word_at(norrath_nil_rep_char, 8) + 1;
        narrow = norrath_nil_rep_char + 16;
    }
    replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(&narrow, 0, 0xffffffffu, literal,
                                                                                                     static_cast<unsigned>(strlen(literal)));
    unsigned char* wide;
    if (word_at(norrath_nil_rep_ushort, 12) != 0) {
        wide = clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(norrath_nil_rep_ushort);
    } else {
        *reinterpret_cast<unsigned*>(norrath_nil_rep_ushort + 8) = word_at(norrath_nil_rep_ushort, 8) + 1;
        wide = norrath_nil_rep_ushort + 16;
    }
    resize__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUs(&wide, word_at(narrow - 16, 0), 0);
    {
        const signed char* from = reinterpret_cast<const signed char*>(narrow);
        const signed char* end = from + word_at(narrow - 16, 0);
        unsigned index = 0;
        while (from != end) {
            if (word_at(wide - 16, 8) >= 2)
                alloc__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0Uib(&wide, word_at(wide - 16, 0), true);
            *reinterpret_cast<unsigned*>(wide - 4) = 1;
            reinterpret_cast<unsigned short*>(wide)[index] = static_cast<unsigned short>(*from);
            from += 1;
            index += 1;
        }
    }
    unsigned char* first = share_wide(wide);
    release(&wide, 2);
    unsigned char* second = share_wide(first);
    replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiUiUs(&second, word_at(second - 16, 0), 0, 1, 0);
    unsigned char* third = share_wide(second);
    release(&second, 2);
    release(&first, 2);
    release(&narrow, 1);
    return third;
}

void onLogin__16VIRealmInterfaceUiiPCUsPv(void* self, unsigned a, int status, const unsigned short* name, void* extra) {
    using namespace norrath_aoc;
    (void)extra;
    unsigned char* object = static_cast<unsigned char*>(self);
    GetAppPtr__5VIWnd(load_pointer(object + 12));
    UnLock__11VIGUIDialog(load_pointer(load_pointer(object + 12) + 56));
    if (status == 0) {
        unsigned short* buffer = reinterpret_cast<unsigned short*>(object + 16);
        strncpyWide__FPUsPCUsUi(buffer, name, 256);
        store_word(object + 272, static_cast<unsigned>(strlenWide__FPCUs(buffer)));
    } else {
        unsigned char* first = wide_login_text("foo");
        unsigned char* second = wide_login_text("bar");
        login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(self, reinterpret_cast<const unsigned short*>(first),
                                                         reinterpret_cast<const unsigned short*>(second), 0);
        release(&second, 2);
        release(&first, 2);
    }
    printf("VIRealmInterface::onLogin(%i, %i)\n", a, status);
}
}
