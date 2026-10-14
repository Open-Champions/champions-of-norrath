// Observed modelSetHalo__FP7HaloDefi at 0x0016cbe8, extent 12. Stores the first
// argument (a pointer) into the word halos (0x00484084) and the second argument
// into the word nmHalos (0x00484088). Returns
// nothing. halos and nmHalos are the two objects modelResetOptionalParams__Fv
// resets, so the pair is a table pointer and a count.
// Inferred: HaloDef is the element type of that table (from the mangled name).
// Unknown: the HaloDef layout. halos and nmHalos are defined by other packages.
extern "C" {
extern const void* halos;
extern int nmHalos;
void modelSetHalo__FP7HaloDefi(const void* table, int count) {
    halos = table;
    nmHalos = count;
}
}
