// Observed _GLOBAL_$I$hudNoDraw at 0x001c8078, 32 bytes, calling the file-local initializer at
// 0x001c7508 (164 bytes) with (1, 0xffff). With priority 0xffff and initialize equal to 1 the
// initializer constructs three 16-byte ResourceMap objects, in this order, with
// __11ResourceMapPcT1(object, group, name):
//   hudBox     (0x007d06a8)  "hud.lmp"    "hudbox.tex"
//   spellBook  (0x007d06b8)  "hud.lmp"    "spellbook.tex"
//   emptySpell (0x007d06c8)  "icons.lmp"  "icblank.tex"
// Any other priority or initialize value touches nothing; there is no destruction half. The executable lists the
// three objects as zero-size entries in its uninitialized data. No other source in the draft defines them, so they
// are defined here with external linkage and their 16-byte size is taken from the spacing of the addresses. What the
// maps are used for is not established here. Fresh compilation is not an original-byte match.
extern "C" {
void __11ResourceMapPcT1(void* map, const char* group, const char* name);

unsigned char hudBox[16];
unsigned char spellBook[16];
unsigned char emptySpell[16];
}

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) __11ResourceMapPcT1(hudBox, "hud.lmp", "hudbox.tex");
    if (initialize == 1) __11ResourceMapPcT1(spellBook, "hud.lmp", "spellbook.tex");
    if (initialize == 1) __11ResourceMapPcT1(emptySpell, "icons.lmp", "icblank.tex");
}

extern "C" void _GLOBAL_$I$hudNoDraw() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
