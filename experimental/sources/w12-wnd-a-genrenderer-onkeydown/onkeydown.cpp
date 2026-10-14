// Observed: OnKeyDown__20VIWndGenericRendereri at 0x010bd6a0 is 1624 bytes. page = the word at this+52, dialog =
// the word at this+56, realm = this+1880; the table call through "obj+120, entry 24" means: take the table word at
// obj+120, add 24 to it, and call the function in the entry's second word with obj plus the entry's first halfword
// and the key. Nothing happens when the word at this+1400 is nonzero. Otherwise:
//  1. Key remap: 27 -> 135, 19 -> 138, 20 -> 139, 18 -> 137, 17 -> 136, 13 -> 134 (others unchanged); call it k.
//  2. When dialog is nonzero and IsOpen__11VIGUIDialog(dialog) is nonzero: table call on dialog with k; end.
//  3. When the word at this+1420 is nonzero: OnKeyDown__9VIWndEditi(this+1424, k); end.
//  4. When Size__Ct6VIList1ZP9VIGUIPage(this+40) is 1, k is 135 and IsOpen(dialog) is zero (called even when dialog
//     is null): ActionOccured__20VIWndGenericRendererPCc(this, "DisconnectConfirmed"); end.
//  5. When k is 132 and strstr(GetName__9VIGUIPage(page), "FindGame") is nonzero: now = GetTime__7VITimer(); if now
//     minus the 64-bit global lastRefreshTime is below 5000, end. Else store now in lastRefreshTime;
//     obj = GetObjectByName__9VIGUIPagePCc(page, "GameName"); when obj is nonzero, __dynamic_cast_2 is called with
//     (entry-0 function of obj's table at obj+148, __tf13VIGUIComboBox, 0, obj + that entry's halfword,
//     __tf11VIGUIObject, obj) and the result is ignored. Then session = GetSessionId__16VIRealmInterface(realm) is read, an
//     inline sequence widens it into a basic_string of 16-bit units, narrows that to a basic_string of chars (setting the
//     rep's shared flag word to 1 as it writes), copies it, drops the temporary, calls printf("Calling matchGame with id
//     %s\n", text or "" when empty), drops the copy, builds a MatchGameAttrs, calls
//     matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv(realm,
//     GetSessionId(realm), 2, 0, 0, -1, &attrs, true, 100, 1024, 0), destroys it with flag 2, and drops the wide string. End.
//     (Otherwise, when k is not 132 or the name has no "FindGame", step 6.)
//  6. strstr(GetName(page), "GameServ") nonzero with k 135 and IsOpen(dialog) zero: Show__11VIGUIDialogPCUsPCc(dialog,
//     text(1835), "CancelSRSGame") and SetText__11VIGUIDialogPCUsb(dialog, text(1836), false); end.
//  7. strstr(GetName(page), "SRSMain") nonzero with k 135: logout__Q212RealmService15RealmGatewayAPIPCUsPv(realm,
//     GetSessionId(realm), 0). Then strstr(GetName(page), "GameClnt") nonzero with k 135: clientLeaveGame__16VIRealmInterface
//     (realm), GoBack__20VIWndGenericRenderer(this).
//  8. When page is nonzero: table call on page with k.
//  text(id) = GetString__13VIStringTableii(mUI, id, word at mUI+48), mUI the word _5VIWnd$mUI (0x01192c70).
// The reference-counted string handling is the image's inlined library code: a handle is a pointer to the characters; the
// 16-byte header before it holds length (+0), capacity (+4), reference count (+8), shared flag (+12) as read here.
// A default string shares the static nil rep (a copy via clone__...3Rep when the nil rep's flag word is nonzero); release
// decrements the count and, at zero, returns the block to the pool free list indexed by ((capacity bytes + 16 + 7) / 8) - 1
// when it is at most 128 bytes, else calls __wrap_free.
// Inferred: the keys are the pad/keyboard codes after translation (132 a find/refresh button, 134 and 135 confirm and back,
// 136 to 139 directions); the shared-flag stores mark the narrowed string unsharable while it is written.
// Unknown: key names, the type behind the dynamic cast (its result is unused), what lastRefreshTime counts, the layout of
// MatchGameAttrs (not used here), and whether the original source wrote a helper for the string narrowing.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
extern unsigned _5VIWnd$mUI;
extern long long lastRefreshTime;
extern unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
extern unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

int IsOpen__11VIGUIDialog(void* dialog);
void OnKeyDown__9VIWndEditi(void* edit, int key);
int Size__Ct6VIList1ZP9VIGUIPage(const void* list);
void ActionOccured__20VIWndGenericRendererPCc(void* self, const char* action);
const char* GetName__9VIGUIPage(void* page);
char* strstr(const char* haystack, const char* needle);
long long GetTime__7VITimer();
void* GetObjectByName__9VIGUIPagePCc(void* page, const char* name);
void __dynamic_cast_2(void* function, const void* target, int offset, void* adjusted, const void* source, void* object);
void __tf13VIGUIComboBox();
void __tf11VIGUIObject();
const unsigned short* GetSessionId__16VIRealmInterface(void* realm);
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(void* rep);
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep);
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(
    void* string, unsigned position, unsigned count, const unsigned short* source, unsigned length);
void resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(void* string,
                                                                                            unsigned length, char fill);
void alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(void* string,
                                                                                            unsigned length, bool flag);
void __wrap_free(void* block);
int printf(const char* format, ...);
void __Q212RealmService14MatchGameAttrs(void* attrs);
void _$_Q212RealmService14MatchGameAttrs(void* attrs, int flags);
void matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv(
    void* api, const unsigned short* session, short a, unsigned b, unsigned c, int d, const void* attrs, bool e,
    unsigned f, unsigned g, void* h);
void logout__Q212RealmService15RealmGatewayAPIPCUsPv(void* api, const unsigned short* session, void* data);
void clientLeaveGame__16VIRealmInterface(void* realm);
void GoBack__20VIWndGenericRenderer(void* self);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);
void Show__11VIGUIDialogPCUsPCc(void* dialog, const unsigned short* text, const char* action);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, bool flag);

#ifdef NORRATH_HOST_ARENA
extern "C" void* norrath_host_function(unsigned address);
struct VtEntry {
    short delta;
    short index;
    unsigned function;
};
#define NORRATH_ENTRY_FUNCTION(entry) reinterpret_cast<void (*)(void*, int)>(norrath_host_function((entry)->function))
#else
struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};
#define NORRATH_ENTRY_FUNCTION(entry) ((entry)->function)
#endif

static inline unsigned word_at(const void* base, int offset) {
    return *reinterpret_cast<const unsigned*>(static_cast<const unsigned char*>(base) + offset);
}
static inline void* pointer_of(unsigned value) {
    return reinterpret_cast<void*>(NORRATH_WORD_ADDRESS(value));
}
static inline void table_call(void* object, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(
        NORRATH_WORD_ADDRESS(word_at(bytes, 120)) + 24);
    NORRATH_ENTRY_FUNCTION(entry)(bytes + entry->delta, key);
}
static inline const unsigned short* text(int id) {
    void* ui = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(ui, id, static_cast<int>(word_at(ui, 48)));
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
        *slot = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(rep));
    } else {
        __wrap_free(rep);
    }
}

void OnKeyDown__20VIWndGenericRendereri(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (word_at(self, 1400) != 0) return;
    if (key == 27) key = 135;
    if (key == 19) key = 138;
    else if (key == 20) key = 139;
    else if (key == 18) key = 137;
    else if (key == 17) key = 136;
    else if (key == 13) key = 134;
    void* dialog = pointer_of(word_at(self, 56));
    if (dialog != 0 && IsOpen__11VIGUIDialog(dialog)) {
        table_call(dialog, key);
        return;
    }
    if (word_at(self, 1420) != 0) {
        OnKeyDown__9VIWndEditi(bytes + 1424, key);
        return;
    }
    if (Size__Ct6VIList1ZP9VIGUIPage(bytes + 40) == 1 && key == 135 && !IsOpen__11VIGUIDialog(dialog)) {
        ActionOccured__20VIWndGenericRendererPCc(self, "DisconnectConfirmed");
        return;
    }
    void* realm = bytes + 1880;
    if (key == 132 && strstr(GetName__9VIGUIPage(pointer_of(word_at(self, 52))), "FindGame") != 0) {
        const long long now = GetTime__7VITimer();
        if (now - lastRefreshTime < 5000) return;
        lastRefreshTime = now;
        void* object = GetObjectByName__9VIGUIPagePCc(pointer_of(word_at(self, 52)), "GameName");
        if (object != 0) {
            const unsigned char* table = static_cast<const unsigned char*>(pointer_of(word_at(object, 148)));
            __dynamic_cast_2(pointer_of(word_at(table, 4)), reinterpret_cast<const void*>(__tf13VIGUIComboBox), 0,
                             static_cast<unsigned char*>(object) + *reinterpret_cast<const short*>(table),
                             reinterpret_cast<const void*>(__tf11VIGUIObject), object);
        }
        const unsigned short* session = GetSessionId__16VIRealmInterface(realm);
        unsigned char* wide;
        if (word_at(norrath_nil_rep_ushort, 12) != 0) {
            wide = clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(
                norrath_nil_rep_ushort);
        } else {
            *reinterpret_cast<unsigned*>(norrath_nil_rep_ushort + 8) = word_at(norrath_nil_rep_ushort, 8) + 1;
            wide = norrath_nil_rep_ushort + 16;
        }
        unsigned length = 0;
        if (session[0] != 0) {
            const unsigned short* cursor = session + 1;
            do {
                length += 1;
            } while (*cursor++ != 0);
        }
        replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(
            &wide, 0, 0xffffffffu, session, length);
        unsigned char* narrow;
        if (word_at(norrath_nil_rep_char, 12) != 0) {
            narrow = clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(
                norrath_nil_rep_char);
        } else {
            *reinterpret_cast<unsigned*>(norrath_nil_rep_char + 8) = word_at(norrath_nil_rep_char, 8) + 1;
            narrow = norrath_nil_rep_char + 16;
        }
        resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(
            &narrow, word_at(wide - 16, 0), 0);
        {
            const unsigned short* from = reinterpret_cast<const unsigned short*>(wide);
            const unsigned short* end = from + word_at(wide - 16, 0);
            unsigned index = 0;
            while (from != end) {
                if (word_at(narrow - 16, 8) >= 2)
                    alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(
                        &narrow, word_at(narrow - 16, 0), true);
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
        printf("Calling matchGame with id %s\n", shown);
        release(&copy, 1);
        unsigned char attrs[64];
        __Q212RealmService14MatchGameAttrs(attrs);
        matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv(
            realm, GetSessionId__16VIRealmInterface(realm), 2, 0, 0, -1, attrs, true, 100, 1024, 0);
        _$_Q212RealmService14MatchGameAttrs(attrs, 2);
        release(&wide, 2);
        return;
    }
    if (strstr(GetName__9VIGUIPage(pointer_of(word_at(self, 52))), "GameServ") != 0 && key == 135 &&
        !IsOpen__11VIGUIDialog(pointer_of(word_at(self, 56)))) {
        Show__11VIGUIDialogPCUsPCc(pointer_of(word_at(self, 56)), text(1835), "CancelSRSGame");
        SetText__11VIGUIDialogPCUsb(pointer_of(word_at(self, 56)), text(1836), false);
        return;
    }
    if (strstr(GetName__9VIGUIPage(pointer_of(word_at(self, 52))), "SRSMain") != 0 && key == 135)
        logout__Q212RealmService15RealmGatewayAPIPCUsPv(realm, GetSessionId__16VIRealmInterface(realm), 0);
    if (strstr(GetName__9VIGUIPage(pointer_of(word_at(self, 52))), "GameClnt") != 0 && key == 135) {
        clientLeaveGame__16VIRealmInterface(realm);
        GoBack__20VIWndGenericRenderer(self);
    }
    void* page = pointer_of(word_at(self, 52));
    if (page != 0) table_call(page, key);
}
}
