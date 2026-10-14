// Observed: _$_19VIFlatFileDirectory at 0x010046a8, 288 bytes, returns nothing. With (this, flags); L is the list embedded at this+24:
//  1. The address of _vt$19VIFlatFileDirectory is stored at this+16.
//  2. it = Begin__Ct6VIList1ZP10VIFlatFile(L). While it differs from End__Ct6VIList1ZP10VIFlatFile(L) (called afresh each pass): E =
//     the word at the node address it, read before the node is erased; it = Erase__t6VIList1ZP10VIFlatFilei(L, it); then, when E is
//     nonzero, the entry at byte offset 8 of E's table (the word at E+16) is called with (E + halfword adjustment, 3).
//  3. _$_t6VIList1ZP10VIFlatFile(L, 2).
//  4. The base-class part of the destructor, inlined: the address of _vt$10VIFlatFile is stored at this+16; a nonzero word at +0 is
//     passed to __wrap_free and cleared; a nonzero word at +4 is passed to __wrap_free and cleared; +8 is set to -1 and +12 to zero.
//  5. When bit 0 of flags is set, __builtin_delete(this) is tail-called.
// Inferred: the directory owns the flat-file objects listed in L and deletes each through its second table entry with flags 3
// (destroy and delete); +0 and +4 are the buffers a flat file frees; the table slot at +16 belongs to the flat-file base class.
// Unknown: the buffers' exact roles and the element object layout beyond the table slot.
#ifdef NORRATH_HOST_CONTRACT
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(address))
#endif

extern "C" {
extern unsigned char vt_VIFlatFileDirectory[] __asm__("_vt$19VIFlatFileDirectory");
extern unsigned char vt_VIFlatFile[] __asm__("_vt$10VIFlatFile");
int Begin__Ct6VIList1ZP10VIFlatFile(const void* list);
int End__Ct6VIList1ZP10VIFlatFile(const void* list);
int Erase__t6VIList1ZP10VIFlatFilei(void* list, int node);
void _$_t6VIList1ZP10VIFlatFile(void* list, int flags);
void __wrap_free(void* pointer);
void __builtin_delete(void* pointer);

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};

void _$_19VIFlatFileDirectory(void* self, int flags) {
    unsigned* words = static_cast<unsigned*>(self);
    unsigned char* list = static_cast<unsigned char*>(self) + 24;
    words[4] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_VIFlatFileDirectory));
    int node = Begin__Ct6VIList1ZP10VIFlatFile(list);
    while (node != End__Ct6VIList1ZP10VIFlatFile(list)) {
        unsigned element = *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(static_cast<unsigned>(node)));
        node = Erase__t6VIList1ZP10VIFlatFilei(list, node);
        if (element != 0) {
            unsigned char* file = reinterpret_cast<unsigned char*>(NORRATH_TRANSLATE(element));
            const VtEntry* entry = reinterpret_cast<const VtEntry*>(NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(file + 16))) + 1;
            entry->function(file + entry->delta, 3);
        }
    }
    _$_t6VIList1ZP10VIFlatFile(list, 2);
    words[4] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_VIFlatFile));
    if (words[0] != 0) {
        __wrap_free(NORRATH_TRANSLATE(words[0]));
        words[0] = 0;
    }
    if (words[1] != 0) {
        __wrap_free(NORRATH_TRANSLATE(words[1]));
        words[1] = 0;
    }
    words[2] = static_cast<unsigned>(-1);
    words[3] = 0;
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
