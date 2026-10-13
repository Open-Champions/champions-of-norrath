// Observed _vt$10bad_typeid at 0x00453810, 32 bytes (weak) in initialized .data: four
// slots, each a zero word followed by a function address. The functions, in order, are
// __tf10bad_typeid, _$_10bad_typeid, what__C9exception (the inherited text accessor), and
// a null slot ending the table.
// Inferred: a virtual table in the layout of this compiler, from the symbol name and
// contents; the zero words are adjustments. Unknown: whether a runtime reader treats the
// first slot as an entry or as a header.
extern "C" {
struct Slot {
    unsigned adjustment;
    void (*function)();
};
void slot_type_info() asm("__tf10bad_typeid");
void slot_destructor() asm("_$_10bad_typeid");
void slot_what() asm("what__C9exception");

extern Slot _vt$10bad_typeid[4];
Slot _vt$10bad_typeid[4] = {
    {0u, slot_type_info},
    {0u, slot_destructor},
    {0u, slot_what},
    {0u, nullptr},
};
}
