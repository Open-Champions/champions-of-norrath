#include "../w9-animation-object-c-common/mem.hpp"
// Observed onGetGame__16VIRealmInterfaceiiPCUsT3iPv at 0x01108508, 1140 bytes. With (this, a, status, address, other, value, extra), the
// fourth argument is never read and neither is extra; offsets are in this:
//  1. printf("VIRealmInterface::onGetGame(%i, %i)\n", a, status).
//  2. status 1: SetText__11VIGUIDialogPCUsb(dialog, string 1861, 0). Any other nonzero status: the same with string 1853. Here dialog is
//     the word at (the word at this+12) + 56 and string n is GetString__13VIStringTableii(the word _5VIWnd$mUI, n, the word at that
//     pointer + 48).
//  3. status 0: AddPage__20VIWndGenericRendererPCc(the word at this+12, "data/ui/GameClnt.xml"); the word at +3880 = value. The wide
//     address string (counted to its terminating zero) is narrowed to a char string through the image's inlined reference-counted string
//     code, exactly as in onGetRules (build a wide string from the shared empty string, replace it with the address, build a narrow
//     string, resize it, store the low byte of each unit, share it into a copy, release the narrow string); the copy's characters (or ""
//     when empty, otherwise terminated in place) are duplicated with __wrap_strdup, then the copy and the wide string are released.
//     In the duplicate, when strstr(dup, ":") finds a colon, the colon is overwritten with a zero byte, the word at +3372 =
//     atoi(the text after it), and snprintf(this+2348, 1023, dup) is called (dup itself is the format). The duplicate is freed with
//     __wrap_free. Then doClientDiscovery__Q212RealmService15RealmGatewayAPIPCUsiT1Pv(this, this+16, the word at +3880, 0, 0);
//     SetStringValue__9VIGUIPagePCcPCUsb(page, "ServerStatus", string 1839, 1) where page is the word at (the word at this+12) + 52;
//     SetStringValue__9VIGUIPagePCcT1b(page, "GameName", this+276, 0); Hide__11VIGUIDialog(dialog).
//  A string handle is a pointer to the characters; the 16-byte header before it holds length (+0), capacity (+4), count (+8), flag
//  (+12). Release drops the count; at zero the block goes on the pool free list indexed by ((capacity * element + 16 + 7) / 8) - 1 when
//  its size is at most 128 bytes, else __wrap_free.
// Inferred: the reply to a request for a game's details; the address text has the form "host:port" (host copied to +2348, port to
// +3372), after which the client connects through the discovery call (whose address text is the name kept at +16).
// Unknown: the status meanings; what the first argument is (printed only); the fourth argument; the roles of the words at +3880 and the
// fifth argument; the string ids' text.
extern "C" {
extern unsigned vi_wnd_mui __asm__("_5VIWnd$mUI");
extern unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
extern unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

int printf(const char* format, ...);
int snprintf(char* out, unsigned long size, const char* format, ...);
char* strstr_call(const char* hay, const char* needle) __asm__("strstr");
int atoi(const char* text);
char* __wrap_strdup(const char* text);
void __wrap_free(void* block);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, int flag);
void Hide__11VIGUIDialog(void* dialog);
void AddPage__20VIWndGenericRendererPCc(void* renderer, const char* path);
void SetStringValue__9VIGUIPagePCcPCUsb(void* page, const char* name, const unsigned short* value, int flag);
void SetStringValue__9VIGUIPagePCcT1b(void* page, const char* name, const char* value, int flag);
void doClientDiscovery__Q212RealmService15RealmGatewayAPIPCUsiT1Pv(void* api, const unsigned short* name, int a, int b, void* c);
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(void* rep);
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep);
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(
    void* string, unsigned position, unsigned count, const unsigned short* source, unsigned length);
void resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(void* string, unsigned length, char fill);
void alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(void* string, unsigned length, bool flag);

static inline unsigned word_at(const void* base, int offset) {
    return *reinterpret_cast<const unsigned*>(static_cast<const unsigned char*>(base) + offset);
}
static const unsigned short* realm_string(int id) {
    using namespace norrath_aoc;
    unsigned char* table = load_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui));
    return GetString__13VIStringTableii(table, id, static_cast<int>(load_word(table + 48)));
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

void onGetGame__16VIRealmInterfaceiiPCUsT3iPv(void* self, int a, int status, const unsigned short* address, const unsigned short* other,
                                              int value, void* extra) {
    using namespace norrath_aoc;
    (void)other;
    (void)extra;
    unsigned char* object = static_cast<unsigned char*>(self);
    printf("VIRealmInterface::onGetGame(%i, %i)\n", a, status);
    if (status != 0) {
        if (status == 1)
            SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), realm_string(1861), 0);
        else
            SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), realm_string(1853), 0);
        return;
    }
    AddPage__20VIWndGenericRendererPCc(load_pointer(object + 12), "data/ui/GameClnt.xml");
    store_word(object + 3880, static_cast<unsigned>(value));
    unsigned char* wide;
    if (word_at(norrath_nil_rep_ushort, 12) != 0) {
        wide = clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(norrath_nil_rep_ushort);
    } else {
        *reinterpret_cast<unsigned*>(norrath_nil_rep_ushort + 8) = word_at(norrath_nil_rep_ushort, 8) + 1;
        wide = norrath_nil_rep_ushort + 16;
    }
    unsigned length = 0;
    if (address[0] != 0) {
        const unsigned short* cursor = address + 1;
        do {
            length += 1;
        } while (*cursor++ != 0);
    }
    replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(&wide, 0, 0xffffffffu, address, length);
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
    char* duplicate = __wrap_strdup(shown);
    release(&copy, 1);
    release(&wide, 2);
    char* colon = strstr_call(duplicate, ":");
    if (colon != 0) {
        *colon = 0;
        store_word(object + 3372, static_cast<unsigned>(atoi(colon + 1)));
        snprintf(reinterpret_cast<char*>(object + 2348), 1023, duplicate);
    }
    __wrap_free(duplicate);
    doClientDiscovery__Q212RealmService15RealmGatewayAPIPCUsiT1Pv(self, reinterpret_cast<const unsigned short*>(object + 16),
                                                                  static_cast<int>(load_word(object + 3880)), 0, 0);
    unsigned char* page = load_pointer(load_pointer(object + 12) + 52);
    SetStringValue__9VIGUIPagePCcPCUsb(page, "ServerStatus", realm_string(1839), 1);
    SetStringValue__9VIGUIPagePCcT1b(load_pointer(load_pointer(object + 12) + 52), "GameName", reinterpret_cast<const char*>(object + 276), 0);
    Hide__11VIGUIDialog(load_pointer(load_pointer(object + 12) + 56));
}
}
