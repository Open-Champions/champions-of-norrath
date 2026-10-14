// Observed: _$_Q212RealmService15RealmGatewayAPI at 0x01124250, 124 bytes, returns nothing. With (this, flags):
//  1. The address of _vt$Q212RealmService15RealmGatewayAPI is stored at this+8.
//  2. P = the word at this+0. When P is nonzero its table pointer (the word at P+100) is read, and the entry at byte offset 8 of that
//     table (a halfword adjustment followed, at +4 of the entry, by a function address) is called with (P + adjustment, 3).
//  3. When bit 0 of flags is set, __builtin_delete(this) is tail-called.
// Inferred: +0 holds an owned object that implements the usual destructor convention (flags 3 = destroy and delete), reached through
// the second table entry; +8 is the table slot of the gateway object; flags bit 0 is the delete-after flag. The word at +0 is not
// cleared afterwards.
// Unknown: the owned object's class and the table slot's meaning.
#ifdef NORRATH_HOST_CONTRACT
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(address))
#endif

extern "C" {
extern unsigned char vt_RealmGatewayAPI[] __asm__("_vt$Q212RealmService15RealmGatewayAPI");
void __builtin_delete(void* pointer);

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};

void _$_Q212RealmService15RealmGatewayAPI(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 8) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_RealmGatewayAPI));
    unsigned char* owned = reinterpret_cast<unsigned char*>(NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(object)));
    if (owned) {
        const VtEntry* entry = reinterpret_cast<const VtEntry*>(NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(owned + 100))) + 1;
        entry->function(owned + entry->delta, 3);
    }
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
