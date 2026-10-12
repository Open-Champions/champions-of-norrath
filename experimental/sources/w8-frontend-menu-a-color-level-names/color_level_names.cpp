// Observed colorLevelNames__FP4Menu at 0x0017ff78, 156 bytes. The argument
// points at an array of 16-byte records whose first word is a label pointer;
// the array ends at the first record whose label is null. The function looks
// up the act number of the string at levelName+81 (inside the 322-byte object
// levelName at 0x0034b660) with MC_NameToAct__FPc, then walks the records.
// For each record it stores a word at offset 4: 7 when the label's act number
// is -1, otherwise 0 when it equals the level-name act number and 6 when not.
// The -1 test comes first, so a record is 7 even when the level-name act is
// also -1. It returns nothing.
// Inferred: offset 4 is a colour index, from the values; nothing else is
// known about the Menu record's remaining words.
extern "C" {
struct norrath_menu_record {
    const char* label;
    int color;
    int third;
    int fourth;
};
int MC_NameToAct__FPc(const char* name);
extern char levelName[];

void colorLevelNames__FP4Menu(norrath_menu_record* menu) {
    int current = MC_NameToAct__FPc(levelName + 81);
    for (; menu->label != 0; ++menu) {
        int act = MC_NameToAct__FPc(menu->label);
        menu->color = act == -1 ? 7 : (act != current ? 6 : 0);
    }
}
}
