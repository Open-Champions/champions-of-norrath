// Observed: _$_5VIWnd at 0x010c3390 is 236 bytes. With (this, flags) it stores the address of
// _vt$5VIWnd at this+32, then walks the set embedded at this: iterator = Begin__Ct5VISet1ZP5VIWnd
// (this); until it equals End__Ct5VISet1ZP5VIWnd(this) (re-evaluated each time), next =
// Next__Ct5VISet1ZP5VIWndi(this, it), then the child pointer is the word at offset 16 of the
// 20-byte element number `it` of the array whose base is the word at this+8. It calls
// RemoveChild__5VIWndP5VIWnd(this, child); when child is nonzero it calls the table entry at
// (table + 8) of the child's table (table word at child+32) with the child adjusted by the
// entry's first halfword and second argument 3. Then it = next. Afterwards it runs
// _$_t5VISet1ZP5VIWnd(this, 2) and, when bit 0 of flags is set, tail-calls __builtin_delete(this).
// Inferred: the table entry at +8 is a destructor taking a delete flag; the iterator is an
// element index (passed as an integer); element stride 20 and key offset 16 come from the
// index scaling.
extern "C" {
struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};
typedef int Iterator;

Iterator Begin__Ct5VISet1ZP5VIWnd(void* set);
Iterator End__Ct5VISet1ZP5VIWnd(void* set);
Iterator Next__Ct5VISet1ZP5VIWndi(void* set, Iterator it);
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void _$_t5VISet1ZP5VIWnd(void* set, int flags);
void __builtin_delete(void* pointer);
extern unsigned char _vt$5VIWnd[];

static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}

void dtor_5VIWnd(void* self, int flags) asm("_$_5VIWnd");
void dtor_5VIWnd(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) = word_of(_vt$5VIWnd);
    Iterator it = Begin__Ct5VISet1ZP5VIWnd(self);
    while (it != End__Ct5VISet1ZP5VIWnd(self)) {
        const Iterator next = Next__Ct5VISet1ZP5VIWndi(self, it);
        unsigned char* elements = static_cast<unsigned char*>(pointer_of(*reinterpret_cast<unsigned*>(bytes + 8)));
        void* child = pointer_of(*reinterpret_cast<unsigned*>(elements + it * 20 + 16));
        RemoveChild__5VIWndP5VIWnd(self, child);
        if (child != 0) {
            unsigned char* object = static_cast<unsigned char*>(child);
            const VtEntry* entry =
                reinterpret_cast<const VtEntry*>(pointer_of(*reinterpret_cast<unsigned*>(object + 32))) + 1;
            entry->function(object + entry->delta, 3);
        }
        it = next;
    }
    _$_t5VISet1ZP5VIWnd(self, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
