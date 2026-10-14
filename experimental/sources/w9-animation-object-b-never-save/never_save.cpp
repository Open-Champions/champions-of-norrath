// Observed neverSave at 0x333d98 in .data, 76 bytes (the next symbol, vu1floorstart,
// begins at 0x333df0 after the table's end and alignment). Nineteen words: eighteen
// name pointers and a null terminator, read by neverSaveAlwaysLoadFromLump__FPc.
// The entries, in table order, are the object-name texts at 0x483b38, 0x483b40,
// 0x483b48, 0x461fc0, 0x483b50, 0x461fd0, 0x461fe0, a "faytree" text, 0x483b60,
// 0x483b68, 0x461ff0, 0x462000, 0x462010, 0x462028, 0x462040, 0x462058, 0x462058
// again (the same address, not a second copy), 0x462068, then zero. The texts at
// those addresses are provided by the object factory units; only the "faytree"
// slot has no separate provider and is a literal here.
// This is the same table the earlier authored norrath_game_never_save_table_view
// holds under another name; this definition is under the executable's own name.
// Unknown: whether a later write replaces any pointer.
extern "C" {
extern const char norrath_factory_text_483b38[];
extern const char norrath_factory_text_483b40[];
extern const char norrath_factory_text_483b48[];
extern const char norrath_factory_text_461fc0[];
extern const char norrath_factory_text_483b50[];
extern const char norrath_factory_text_461fd0[];
extern const char norrath_factory_text_461fe0[];
extern const char norrath_factory_text_483b60[];
extern const char norrath_factory_text_483b68[];
extern const char norrath_factory_text_461ff0[];
extern const char norrath_factory_text_462000[];
extern const char norrath_factory_text_462010[];
extern const char norrath_factory_text_462028[];
extern const char norrath_factory_text_462040[];
extern const char norrath_factory_text_462058[];
extern const char norrath_factory_text_462068[];

const char* neverSave[19] = {
    norrath_factory_text_483b38, norrath_factory_text_483b40, norrath_factory_text_483b48, norrath_factory_text_461fc0,
    norrath_factory_text_483b50, norrath_factory_text_461fd0, norrath_factory_text_461fe0, "faytree",
    norrath_factory_text_483b60, norrath_factory_text_483b68, norrath_factory_text_461ff0, norrath_factory_text_462000,
    norrath_factory_text_462010, norrath_factory_text_462028, norrath_factory_text_462040, norrath_factory_text_462058,
    norrath_factory_text_462058, norrath_factory_text_462068, nullptr,
};
}
#if defined(__mips__)
static_assert(sizeof(neverSave) == 76, "eighteen pointers and one null word");
#endif
