// Observed: OnDraw__5VIWnd at 0x010c3130 is 196 bytes. With this: when this equals the global
// sWndMcErr (0x01190a20) it calls OnDrawSelf__10VIWndMcErr(this); otherwise it calls its own
// table entry at byte offset 24 (table word at this+32), adjusted by the entry's first halfword.
// Then it iterates the child set embedded at this: it = Begin__Ct5VISet1ZP5VIWnd(this); until it
// equals End__Ct5VISet1ZP5VIWnd(this) (re-evaluated each pass) it reads the child pointer at
// element offset 16 of the 20-byte element it (array base at this+8); when the word at child+28
// is nonzero it calls the child's table entry at byte offset 16 (adjusted by the halfword);
// it = Next__Ct5VISet1ZP5VIWndi(this, it).
// Inferred: entry 24 is OnDrawSelf and entry 16 is OnDraw in the class tables (see the splash
// table, slots 2 and 3), so this draws the window, then each visible child recursively; the word
// at child+28 is a visibility flag.
// Unknown: the flag's exact name and semantics.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
extern unsigned char sWndMcErr[0x210];
void OnDrawSelf__10VIWndMcErr(void* self);
int Begin__Ct5VISet1ZP5VIWnd(void* set);
int End__Ct5VISet1ZP5VIWnd(void* set);
int Next__Ct5VISet1ZP5VIWndi(void* set, int it);

#ifdef NORRATH_HOST_ARENA
extern "C" void* norrath_host_function(unsigned address);
struct VtEntry {
    short delta;
    short index;
    unsigned function;
};
#define NORRATH_ENTRY_FUNCTION(entry) reinterpret_cast<void (*)(void*)>(norrath_host_function((entry)->function))
#else
struct VtEntry {
    short delta;
    short index;
    void (*function)(void*);
};
#define NORRATH_ENTRY_FUNCTION(entry) ((entry)->function)
#endif

static inline void call_entry(void* object, int byte_offset) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(
        NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(bytes + 32)) + byte_offset);
    NORRATH_ENTRY_FUNCTION(entry)(bytes + entry->delta);
}

void OnDraw__5VIWnd(void* self) {
    if (self == sWndMcErr) OnDrawSelf__10VIWndMcErr(self);
    else call_entry(self, 24);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    for (int it = Begin__Ct5VISet1ZP5VIWnd(self); it != End__Ct5VISet1ZP5VIWnd(self);
         it = Next__Ct5VISet1ZP5VIWndi(self, it)) {
        unsigned char* elements = reinterpret_cast<unsigned char*>(
            NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(bytes + 8)));
        unsigned char* child = reinterpret_cast<unsigned char*>(
            NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(elements + it * 20 + 16)));
        if (*reinterpret_cast<unsigned*>(child + 28) != 0) call_entry(child, 16);
    }
}
}
