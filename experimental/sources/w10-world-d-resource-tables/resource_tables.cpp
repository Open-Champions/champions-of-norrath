// skeletonResMap (0x0042d8d0, 800 bytes, .data, file-backed) and typeToResource (0x00422a10,
// 40 bytes, .data, file-backed).
// skeletonResMap is 100 eight-byte entries: a pointer to a file-name string and a word that is zero
// in the image, ending with an all-zero entry. The 99 names are the animation, model and texture
// file names for the skeleton resource set (observed text). The pointed-to strings are 16 bytes
// apart in the image; here the compiler places the literals.
// typeToResource is ten pointers to per-race resource maps: Barbarian, Wood Elf, High Elf, Erudite
// and Dark Elf, each male then female, in the order maleBarb, femBarb, maleWoodElf,
// femaleWoodElf, maleHighElf, femaleHighElf, maleErudite, femaleErudite, maleDarkElf,
// femaleDarkElf. The target objects (the "...ResourceMap" symbols) are not defined by this unit.
// Inferred: indexed by a race/sex type number. Unknown: the second word of a skeletonResMap entry,
// and the target objects' layout.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
struct ResourceEntry {
    const char* name;
    int value;
};
extern "C" {
extern unsigned char maleBarbResourceMap[];
extern unsigned char femBarbResourceMap[];
extern unsigned char maleWoodElfResourceMap[];
extern unsigned char femaleWoodElfResourceMap[];
extern unsigned char maleHighElfResourceMap[];
extern unsigned char femaleHighElfResourceMap[];
extern unsigned char maleEruditeResourceMap[];
extern unsigned char femaleEruditeResourceMap[];
extern unsigned char maleDarkElfResourceMap[];
extern unsigned char femaleDarkElfResourceMap[];

ResourceEntry skeletonResMap[100] NORRATH_LOADED(".data") = {
    {"skelcom.lmp", 0},
    {"1_ataka.anm", 0},
    {"1_atakb.anm", 0},
    {"1_atakc.anm", 0},
    {"1_blockhit.anm", 0},
    {"1_blok.anm", 0},
    {"1_hitreact.anm", 0},
    {"1_idlea.anm", 0},
    {"1_idleb.anm", 0},
    {"1_knokbk.anm", 0},
    {"1_march.anm", 0},
    {"1_rally.anm", 0},
    {"1_run.anm", 0},
    {"1_walk.anm", 0},
    {"2_ataka.anm", 0},
    {"2_ataka.anm", 0},
    {"2_atakb.anm", 0},
    {"2_blockhit.anm", 0},
    {"2_block.anm", 0},
    {"2_hitreact.anm", 0},
    {"2_idlea.anm", 0},
    {"2_idleb.anm", 0},
    {"2_knokbk.anm", 0},
    {"2_march.anm", 0},
    {"2_rally.anm", 0},
    {"2_run.anm", 0},
    {"2_walk.anm", 0},
    {"3_atakb.anm", 0},
    {"3_atakb.anm", 0},
    {"3_ataka.anm", 0},
    {"3_blockhit.anm", 0},
    {"3_block.anm", 0},
    {"3_hitreact.anm", 0},
    {"3_idlea.anm", 0},
    {"3_idleb.anm", 0},
    {"3_knokbk.anm", 0},
    {"3_march.anm", 0},
    {"3_rally.anm", 0},
    {"3_run.anm", 0},
    {"3_walk.anm", 0},
    {"4_atakb.anm", 0},
    {"4_atakb.anm", 0},
    {"4_ataka.anm", 0},
    {"4_blockhit.anm", 0},
    {"4_block.anm", 0},
    {"4_hitreact.anm", 0},
    {"4_idlea.anm", 0},
    {"4_idleb.anm", 0},
    {"4_knokbk.anm", 0},
    {"4_march.anm", 0},
    {"4_rally.anm", 0},
    {"4_run.anm", 0},
    {"4_walk.anm", 0},
    {"5_ataka.anm", 0},
    {"5_atakb.anm", 0},
    {"5_ataka.anm", 0},
    {"5_hitreact.anm", 0},
    {"5_idlea.anm", 0},
    {"5_hitreact.anm", 0},
    {"5_idlea.anm", 0},
    {"5_idleb.anm", 0},
    {"5_bkstep.anm", 0},
    {"5_march.anm", 0},
    {"5_idlea.anm", 0},
    {"5_run.anm", 0},
    {"5_walk.anm", 0},
    {"1_command.anm", 0},
    {"4_bkstep.anm", 0},
    {"death1.anm", 0},
    {"death2.anm", 0},
    {"death3.anm", 0},
    {"death4.anm", 0},
    {"death5.anm", 0},
    {"hop_l.anm", 0},
    {"hop_r.anm", 0},
    {"stumble_a.anm", 0},
    {"stumble_b.anm", 0},
    {"1_axe.vif", 0},
    {"1_axe.tex", 0},
    {"swd1.vif", 0},
    {"swd1.tex", 0},
    {"arrow.vif", 0},
    {"arrow.tex", 0},
    {"bow.vif", 0},
    {"bow.tex", 0},
    {"dhaxe1.vif", 0},
    {"dhaxe1.tex", 0},
    {"dhaxe2.vif", 0},
    {"dhaxe2.tex", 0},
    {"dhswd1.vif", 0},
    {"dhswd1.tex", 0},
    {"shield1.vif", 0},
    {"shield1.tex", 0},
    {"shield2.vif", 0},
    {"shield2.tex", 0},
    {"spear1.vif", 0},
    {"spear1.tex", 0},
    {"spear2.vif", 0},
    {"spear2.tex", 0},
    {0, 0},
};

void* typeToResource[10] NORRATH_LOADED(".data") = {
    maleBarbResourceMap, femBarbResourceMap, maleWoodElfResourceMap, femaleWoodElfResourceMap,
    maleHighElfResourceMap, femaleHighElfResourceMap, maleEruditeResourceMap, femaleEruditeResourceMap,
    maleDarkElfResourceMap, femaleDarkElfResourceMap,
};
}
