// Observed: gameInitPostFrontend__Fv at 0x001cd2b0 is 304 bytes (the symbol is global). It loads
// the lump "icons.lmp" with lumpLoad__FPc, then for ten names in this order calls
// lumpFind__FPvPc(lump, name) and stores the word at byte 4 of the result into the word of
// globalIcons (0x0042a418, 152 bytes) at the byte offset shown:
//   "noarrows.tex" 120, "nohealth.tex" 124, "nomana.tex" 128, "nocamera.tex" 132,
//   "countarrows.tex" 144, "counthealth.tex" 136, "countmana.tex" 140, "icslam.tex" 148,
//   "icfroststorm.tex" 112, "icroot.tex" 100.
// Then HUDInit__Fv() is called. Nothing is returned. In the original the stores trail their
// lookup by one call (they sit in branch delay slots); the order of effects is unchanged.
// Inferred: the lump holds the interface icon textures and byte 4 of a found entry is the
// resource the HUD later draws with.
// Unknown: the role of the other words of globalIcons, and the layout of a lump entry.

extern "C" {
extern unsigned char globalIcons[];
void* lumpLoad__FPc(const char* name);
unsigned char* lumpFind__FPvPc(void* lump, const char* name);
void HUDInit__Fv();
}

namespace {
void store(void* lump, const char* name, int offset) {
    *reinterpret_cast<unsigned*>(globalIcons + offset) = *reinterpret_cast<unsigned*>(lumpFind__FPvPc(lump, name) + 4);
}
}

extern "C" void gameInitPostFrontend__Fv() {
    void* lump = lumpLoad__FPc("icons.lmp");
    store(lump, "noarrows.tex", 120);
    store(lump, "nohealth.tex", 124);
    store(lump, "nomana.tex", 128);
    store(lump, "nocamera.tex", 132);
    store(lump, "countarrows.tex", 144);
    store(lump, "counthealth.tex", 136);
    store(lump, "countmana.tex", 140);
    store(lump, "icslam.tex", 148);
    store(lump, "icfroststorm.tex", 112);
    store(lump, "icroot.tex", 100);
    HUDInit__Fv();
}
