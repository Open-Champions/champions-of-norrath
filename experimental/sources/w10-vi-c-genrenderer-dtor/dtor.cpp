// Observed: _$_20VIWndGenericRenderer at 0x010b91f8 is 392 bytes. With (this, flags) it:
//  1. stores the address of _vt$20VIWndGenericRenderer (0x012094a8) at this+32;
//  2. if the word at this+36 is nonzero, calls RemoveChild__5VIWndP5VIWnd(this, child), re-reads
//     the word, and if it is still nonzero calls the table entry at (table + 8) of the object's
//     table (table word at object+32) with the object adjusted by the entry's first halfword and
//     second argument 3; then stores 0 at this+36;
//  3. walks the list at this+40: iterator = Begin__Ct6VIList1ZP9VIGUIPage(list); until it equals
//     End__Ct6VIList1ZP9VIGUIPage(list) (re-evaluated each time): the first word of the
//     element at the iterator is a page pointer; if nonzero, its table entry at (table + 8)
//     (table word at page+120) is called with the page adjusted by the entry halfword and second
//     argument 3; then next = Next__Ct6VIList1ZP9VIGUIPagei(list, it), Erase__t6VIList1ZP9VIGUIPagei(
//     list, it), it = next;
//  4. stores 0 at this+52 and, if the word at this+56 is nonzero, calls its (table + 8) entry
//     (table word at +120, second argument 3) and stores 0 at this+56;
//  5. runs _$_16VIRealmInterface(this+1880, 2), _$_9VIWndEdit(this+1424, 2),
//     _$_19VIFlatFileStructure(this+1364, 2), _$_t6VIList1ZP9VIGUIPage(this+40, 2),
//     _$_5VIWnd(this, 0);
//  6. when bit 0 of flags is set, tail-calls __builtin_delete(this).
// Inferred: the table entries are {halfword adjustment, halfword, function} records and the
// entry at +8 is a destructor taking a delete flag; the +120 table word belongs to a
// VIGUIPage-like class. Unknown: the exact class of the objects at this+36 and this+56.
extern "C" {
struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};
typedef __INTPTR_TYPE__ Iterator;

void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
Iterator Begin__Ct6VIList1ZP9VIGUIPage(void* list);
Iterator End__Ct6VIList1ZP9VIGUIPage(void* list);
Iterator Next__Ct6VIList1ZP9VIGUIPagei(void* list, Iterator it);
void Erase__t6VIList1ZP9VIGUIPagei(void* list, Iterator it);
void _$_16VIRealmInterface(void* self, int flags);
void _$_9VIWndEdit(void* self, int flags);
void _$_19VIFlatFileStructure(void* self, int flags);
void _$_t6VIList1ZP9VIGUIPage(void* self, int flags);
void _$_5VIWnd(void* self, int flags);
void __builtin_delete(void* pointer);
extern unsigned char _vt$20VIWndGenericRenderer[];

static void destroy_through_table(void* object, int table_offset) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    const VtEntry* entry = *reinterpret_cast<VtEntry* const*>(bytes + table_offset) + 1;
    entry->function(bytes + entry->delta, 3);
}

void dtor_20VIWndGenericRenderer(void* self, int flags) asm("_$_20VIWndGenericRenderer");
void dtor_20VIWndGenericRenderer(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$20VIWndGenericRenderer));
    void* child = *reinterpret_cast<void**>(bytes + 36);
    if (child != 0) {
        RemoveChild__5VIWndP5VIWnd(self, child);
        child = *reinterpret_cast<void**>(bytes + 36);
        if (child != 0) destroy_through_table(child, 32);
        *reinterpret_cast<void**>(bytes + 36) = 0;
    }
    void* const list = bytes + 40;
    Iterator it = Begin__Ct6VIList1ZP9VIGUIPage(list);
    while (it != End__Ct6VIList1ZP9VIGUIPage(list)) {
        void* page = *reinterpret_cast<void**>(it);
        if (page != 0) destroy_through_table(page, 120);
        const Iterator next = Next__Ct6VIList1ZP9VIGUIPagei(list, it);
        Erase__t6VIList1ZP9VIGUIPagei(list, it);
        it = next;
    }
    *reinterpret_cast<unsigned*>(bytes + 52) = 0;
    void* page = *reinterpret_cast<void**>(bytes + 56);
    if (page != 0) {
        destroy_through_table(page, 120);
        *reinterpret_cast<void**>(bytes + 56) = 0;
    }
    _$_16VIRealmInterface(bytes + 1880, 2);
    _$_9VIWndEdit(bytes + 1424, 2);
    _$_19VIFlatFileStructure(bytes + 1364, 2);
    _$_t6VIList1ZP9VIGUIPage(list, 2);
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
