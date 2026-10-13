// Observed _vt$9bad_alloc at 0x00453950, 32 bytes (weak) in initialized .data: four slots,
// each a zero word followed by a function address. The functions, in order, are
// __tf9bad_alloc, _$_9bad_alloc, what__C9bad_alloc, and a null slot ending the table.
// Inferred: a virtual table in the layout of this compiler, from the symbol name and
// contents; the zero words are adjustments. Unknown: whether a runtime reader treats the
// first slot as an entry or as a header.
extern "C" {
struct Slot {
    unsigned adjustment;
    void (*function)();
};
void slot_type_info() asm("__tf9bad_alloc");
void slot_destructor() asm("_$_9bad_alloc");
void slot_what() asm("what__C9bad_alloc");

extern Slot _vt$9bad_alloc[4];
Slot _vt$9bad_alloc[4] = {
    {0u, slot_type_info},
    {0u, slot_destructor},
    {0u, slot_what},
    {0u, nullptr},
};
}
