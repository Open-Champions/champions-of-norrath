// Observed: OnKeyUp__20VIWndGenericRendereri at 0x010be9d0 is 192 bytes. With (this, key): nothing
// happens when the word at this+1400 is nonzero, or when the word at this+52 (the current page) is zero.
// Otherwise, with d the word at this+56 (the dialog): if d is nonzero and IsOpen__11VIGUIDialog(d) is
// nonzero, the dialog's table entry at byte offset 16 (table word at d+120) is called with the dialog
// adjusted by the entry's first halfword and key, and the routine ends. Else if the word at this+1420
// is nonzero, OnKeyUp__9VIWndEditi(this+1424, key) is called. Else the page's table entry at byte
// offset 16 (table word at page+120) is called the same way with the page and key.
// Inferred: entry 16 of those tables is the key-release handler of the GUI object classes (the
// matching OnKeyDown routine uses offset 24); this+1400 blocks input; this+1420 is an edit-active flag
// and this+1424 an embedded edit window.
// Unknown: the exact names of the flag words.
extern "C" {
int IsOpen__11VIGUIDialog(void* dialog);
void OnKeyUp__9VIWndEditi(void* edit, int key);

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};

static inline unsigned word_at(const void* base, int offset) {
    return *reinterpret_cast<const unsigned*>(static_cast<const unsigned char*>(base) + offset);
}
static inline void call_up(void* object, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(
        static_cast<__UINTPTR_TYPE__>(word_at(bytes, 120)) + 16);
    entry->function(bytes + entry->delta, key);
}

void OnKeyUp__20VIWndGenericRendereri(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (word_at(bytes, 1400) != 0) return;
    void* page = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word_at(bytes, 52)));
    if (page == 0) return;
    void* dialog = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word_at(bytes, 56)));
    if (dialog != 0 && IsOpen__11VIGUIDialog(dialog)) {
        call_up(dialog, key);
        return;
    }
    if (word_at(bytes, 1420) != 0) {
        OnKeyUp__9VIWndEditi(bytes + 1424, key);
        return;
    }
    call_up(page, key);
}
}
