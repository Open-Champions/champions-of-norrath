// Observed: texStrings at 0x0042e278 is ten pointers (40 bytes), each to a
// short resource name ending in ".tex". Callers index it with the player race
// byte or with a word of a player record. The first two names have the same
// first letters followed by m and f, and so on in pairs; the pairing is
// observed in the names only.
// Inferred: none. Unknown: what each texture shows and whether the order is
// race then sex.
extern "C" {
const char* texStrings[10] = {
    "b_bm.tex", "b_bf.tex", "b_wem.tex", "b_wef.tex", "b_hem.tex",
    "b_hef.tex", "b_em.tex", "b_ef.tex", "b_dem.tex", "b_def.tex",
};
}
