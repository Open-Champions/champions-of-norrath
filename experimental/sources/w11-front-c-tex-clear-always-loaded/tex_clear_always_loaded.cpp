// Observed texClearAlwaysLoadedPages__Fv at 0x00172940, 8 bytes, 1 call site
// (worldFreeWorld). It stores zero in the word nmAlwaysPages (gp-4204) and
// returns. Unknown: what the original source called that count's owner.
extern "C" int nmAlwaysPages;

extern "C" void texClearAlwaysLoadedPages__Fv() {
    nmAlwaysPages = 0;
}
