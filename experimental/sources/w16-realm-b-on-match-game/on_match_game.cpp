#include "../w9-animation-object-c-common/mem.hpp"
// Observed onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv at 0x01107790, 3444 bytes (a 5392-byte frame). With
// (this, a, status, count, list4, list5, unused, ids, blobs, extra): list4 and list5 (the fourth and fifth arguments) are arrays of
// count wide-string pointers, unused (the sixth) is never read, ids is an array of count ints, blobs an array of count 8-byte records (a data pointer, then a
// size word), extra is never read. Offsets are in this; page = the word at (the word at this+12) + 52, dialog = the same +56; string n =
// GetString__13VIStringTableii(the word _5VIWnd$mUI, n, the word at that pointer + 48):
//  1. printf("VIRealmInterface::onMatchGame(%i, %i, %i)\n", a, status, count).
//  2. status == 1: SetText__11VIGUIDialogPCUsb(dialog, string 1861, 0). Status neither 0 nor 1: SetText of string 1852 only.
//  3. status == 0 and count == 0: the combo box (see 5) is cleared when found, then SetText of string 1852.
//  4. status == 0 and count != 0: when strstr(GetName__9VIGUIPage(page), "FindGame") is null, snprintf16__8VIStringPUsiPCUse(buffer, 4095,
//     string 1851, count), SetText(dialog, buffer, 0) and AddPage__20VIWndGenericRendererPCc(the word at this+12,
//     "data/ui/FindGame.xml") are done first.
//  5. combo = the result of the dynamic cast of GetObjectByName__9VIGUIPagePCc(page, "GameName") exactly as in OnKeyDown (zero when
//     the object is missing). Its word at +272 (read before the box is cleared) is the selected index; selected = the word at
//     this+3480+4*index, or -1 when there is no combo or the index is negative. When combo exists, Clear__13VIGUIComboBox(combo).
//     memset(this+0x8b1c, 0, 2400). With no combo the call ends. Otherwise, for each i below count, with row text area this+9992+256*i
//     and 24-byte area this+0x8b1c+24*i:
//       a. The wide string list5[i] is narrowed through the image's inlined reference-counted string code (see below); hosttext =
//          blobs[i].data. strstr(hosttext, ":") and, when found, the colon is overwritten with 0 and `rest` = the text after it, else
//          rest = null. sprintf(line, "%s (%s)", narrowed, blobs[i].size != 0 ? hosttext : "?"). If rest is not null, strcpy(this+9992+256*i,
//          rest), else that byte is set to 0. When blobs[i].size >= 220, strcpy(this+0x8b1c+24*i, blobs[i].data + 220).
//          AddOption__13VIGUIComboBoxPc(combo, line). The word at this+3480+4*i = ids[i]; when it equals `selected`,
//          SetValue__13VIGUIComboBoxi(combo, i) and match = i (match starts at 0).
//       b. list4[i] is narrowed and duplicated with __wrap_strdup; when strstr(duplicate, ":") is found the colon is overwritten with 0,
//          the word at this+3884+4*i = atoi(text after it), and the narrow string handle at this+4284+4*i is replaced with the
//          duplicate (strlen after the cut). The duplicate is then freed with __wrap_free.
//       c. list5[i] is narrowed again and assigned into the narrow string handle at this+4684+4*i (the old contents are
//          released, the new one shared); the working strings are released.
//     Finally SetStringValue__9VIGUIPagePCcT1b(page, "HelpText", this+9992+256*match, 0).
//  A string handle is a pointer to the characters; the 16-byte header before it holds length (+0), capacity (+4), count (+8), flag (+12).
//  Narrowing a wide string: a wide handle is built from the shared empty wide string (clone via clone__...3Rep when the rep's flag word is
//  nonzero, else its count is incremented) and replaced with (pointer, length); a narrow handle is built from the shared empty narrow string,
//  resized to that length, and each unit's low byte stored (before each store, when the rep's count is at least 2 it is made private with
//  alloc(..., length, true); the flag word is set to 1); it is then shared into a copy (clone when the flag is nonzero, else the count is
//  incremented) and the narrow string released. Release drops the count; at zero the block goes on the pool free list indexed by
//  ((capacity * element + 16 + 7) / 8) - 1 when its size is at most 128 bytes, else __wrap_free.
// The array arguments are taken as raw bases because their elements are 32-bit target pointer words.
// Inferred: the reply to a game search listing: list4/list5/ids/blobs are parallel result lists, loaded into a combo box and parallel
// per-row tables; the blob's size and the 220-byte offset hint at a fixed header followed by a text field.
// Unknown: the meaning of status values; the layouts of the blob records beyond the pointer and size words; what the tables at +4284 and +4684
// hold downstream; the unused sixth argument.
extern "C" {
extern unsigned vi_wnd_mui __asm__("_5VIWnd$mUI");
extern unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
extern unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

int printf(const char* format, ...);
int sprintf(char* out, const char* format, ...);
void* memset(void* dest, int value, unsigned long size);
char* strcpy(char* dest, const char* source);
unsigned long strlen(const char* text);
char* strstr_call(const char* hay, const char* needle) __asm__("strstr");
int atoi(const char* text);
char* __wrap_strdup(const char* text);
void __wrap_free(void* block);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, int flag);
int snprintf16__8VIStringPUsiPCUse(unsigned short* out, int count, const unsigned short* format, ...);
void AddPage__20VIWndGenericRendererPCc(void* renderer, const char* path);
const char* GetName__9VIGUIPage(void* page);
void* GetObjectByName__9VIGUIPagePCc(void* page, const char* name);
void* __dynamic_cast_2(void* function, const void* target, int offset, void* adjusted, const void* source, void* object);
void __tf13VIGUIComboBox();
void __tf11VIGUIObject();
void Clear__13VIGUIComboBox(void* combo);
void AddOption__13VIGUIComboBoxPc(void* combo, const char* text);
void SetValue__13VIGUIComboBoxi(void* combo, int value);
void SetStringValue__9VIGUIPagePCcT1b(void* page, const char* name, const char* value, int flag);
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(void* rep);
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep);
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(
    void* string, unsigned position, unsigned count, const unsigned short* source, unsigned length);
void replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(
    void* string, unsigned position, unsigned count, const char* source, unsigned length);
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
static inline unsigned char* share_narrow(unsigned char* handle) {
    if (word_at(handle - 16, 12) != 0)
        return clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(handle - 16);
    *reinterpret_cast<unsigned*>(handle - 8) = word_at(handle - 16, 8) + 1;
    return handle;
}
// A wide string handle holding a copy of text (counted to its terminating zero).
static unsigned char* wide_copy(const unsigned short* text) {
    unsigned char* wide;
    if (word_at(norrath_nil_rep_ushort, 12) != 0) {
        wide = clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(norrath_nil_rep_ushort);
    } else {
        *reinterpret_cast<unsigned*>(norrath_nil_rep_ushort + 8) = word_at(norrath_nil_rep_ushort, 8) + 1;
        wide = norrath_nil_rep_ushort + 16;
    }
    unsigned length = 0;
    if (text[0] != 0) {
        const unsigned short* cursor = text + 1;
        do {
            length += 1;
        } while (*cursor++ != 0);
    }
    replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(&wide, 0, 0xffffffffu, text, length);
    return wide;
}
// A narrow string handle holding the low bytes of a wide string, shared into a copy; the working narrow string is released.
static unsigned char* narrow_copy(unsigned char* wide) {
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
    unsigned char* copy = share_narrow(narrow);
    release(&narrow, 1);
    return copy;
}

// The combo box behind the page's "GameName" object, or null.
static void* find_combo(unsigned char* page) {
    using namespace norrath_aoc;
    unsigned char* object = static_cast<unsigned char*>(GetObjectByName__9VIGUIPagePCc(page, "GameName"));
    if (object == 0) return 0;
    const unsigned char* table = load_pointer(object + 148);
    return __dynamic_cast_2(load_pointer(const_cast<unsigned char*>(table) + 4), reinterpret_cast<const void*>(__tf13VIGUIComboBox), 0,
                            object + *reinterpret_cast<const short*>(table), reinterpret_cast<const void*>(__tf11VIGUIObject), object);
}

void onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv(void* self, int a, int status, unsigned count,
                                                                              const unsigned char* list4,
                                                                              const unsigned char* list5,
                                                                              const unsigned char* unused, const int* ids,
                                                                              const unsigned char* blobs, void* extra) {
    using namespace norrath_aoc;
    (void)unused;
    (void)extra;
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned short text[2048];
    char line[1024];
    printf("VIRealmInterface::onMatchGame(%i, %i, %i)\n", a, status, static_cast<int>(count));
    if (status != 0 || count == 0) {
        if (status == 1) {
            SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), realm_string(1861), 0);
            return;
        }
        if (status == 0) {
            void* combo = find_combo(load_pointer(load_pointer(object + 12) + 52));
            if (combo != 0) Clear__13VIGUIComboBox(combo);
        }
        SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), realm_string(1852), 0);
        return;
    }
    if (strstr_call(GetName__9VIGUIPage(load_pointer(load_pointer(object + 12) + 52)), "FindGame") == 0) {
        snprintf16__8VIStringPUsiPCUse(text, 4095, realm_string(1851), count);
        SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), text, 0);
        AddPage__20VIWndGenericRendererPCc(load_pointer(object + 12), "data/ui/FindGame.xml");
    }
    void* combo = find_combo(load_pointer(load_pointer(object + 12) + 52));
    int index = -1;
    if (combo != 0) {
        index = static_cast<int>(word_at(combo, 272));
        Clear__13VIGUIComboBox(combo);
    }
    int selected = -1;
    if (index >= 0) selected = static_cast<int>(word_at(object, 3480 + 4 * index));
    memset(object + 0x8b1c, 0, 2400);
    if (combo == 0) return;
    unsigned match = 0;
    for (unsigned i = 0; i < count; ++i) {
        const unsigned char* blob = blobs + 8 * i;
        unsigned char* wide = wide_copy(reinterpret_cast<const unsigned short*>(load_pointer(const_cast<unsigned char*>(list5) + 4 * i)));
        char* data = reinterpret_cast<char*>(load_pointer(const_cast<unsigned char*>(blob)));
        char* rest = 0;
        char* colon = strstr_call(data, ":");
        if (colon != 0) {
            *colon = 0;
            rest = colon + 1;
        }
        unsigned char* narrow = narrow_copy(wide);
        const char* shown = "";
        if (word_at(narrow - 16, 0) != 0) {
            narrow[word_at(narrow - 16, 0)] = 0;
            shown = reinterpret_cast<const char*>(narrow);
        }
        sprintf(line, "%s (%s)", shown, load_word(blob + 4) != 0 ? data : "?");
        release(&narrow, 1);
        if (rest != 0)
            strcpy(reinterpret_cast<char*>(object + 9992 + 256 * i), rest);
        else
            object[9992 + 256 * i] = 0;
        if (load_word(blob + 4) >= 220) strcpy(reinterpret_cast<char*>(object + 0x8b1c + 24 * i), data + 220);
        AddOption__13VIGUIComboBoxPc(combo, line);
        store_word(object + 3480 + 4 * i, static_cast<unsigned>(ids[i]));
        if (selected == ids[i]) {
            SetValue__13VIGUIComboBoxi(combo, static_cast<int>(i));
            match = i;
        }
        {
            unsigned char* second = wide_copy(reinterpret_cast<const unsigned short*>(load_pointer(const_cast<unsigned char*>(list4) + 4 * i)));
            unsigned char* copy = narrow_copy(second);
            const char* name = "";
            if (word_at(copy - 16, 0) != 0) {
                copy[word_at(copy - 16, 0)] = 0;
                name = reinterpret_cast<const char*>(copy);
            }
            char* duplicate = __wrap_strdup(name);
            release(&copy, 1);
            release(&second, 2);
            char* cut = strstr_call(duplicate, ":");
            if (cut != 0) {
                *cut = 0;
                store_word(object + 3884 + 4 * i, static_cast<unsigned>(atoi(cut + 1)));
                replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(
                    object + 4284 + 4 * i, 0, 0xffffffffu, duplicate, static_cast<unsigned>(strlen(duplicate)));
            }
            __wrap_free(duplicate);
        }
        {
            unsigned char* third = wide_copy(reinterpret_cast<const unsigned short*>(load_pointer(const_cast<unsigned char*>(list5) + 4 * i)));
            unsigned char* copy = narrow_copy(third);
            unsigned char* slot = object + 4684 + 4 * i;
            if (reinterpret_cast<unsigned char*>(&copy) != slot) {
                unsigned char* old = load_pointer(slot);
                release(&old, 1);
                store_pointer(slot, share_narrow(copy));
            }
            release(&copy, 1);
            release(&third, 2);
        }
        release(&wide, 2);
    }
    SetStringValue__9VIGUIPagePCcT1b(load_pointer(load_pointer(object + 12) + 52), "HelpText", reinterpret_cast<const char*>(object + 9992 + 256 * match), 0);
}
}
