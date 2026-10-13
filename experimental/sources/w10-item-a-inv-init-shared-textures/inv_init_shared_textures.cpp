// Observed invInitSharedInterfaceTextures__Fv at 0x001afdd0, 992 bytes. Loads three
// lumps and caches texture pointers (the word at +4 of each lumpFind__FPvPc result).
//   * inventoryLump = lumpLoad__FPc("inventry.lmp"); iconLump = lumpLoad("icons.lmp");
//     hudLump = lumpLoad("hud.lmp") (the third string is in the image's data area).
//   * i = 0..6: moneyTex[i] from "gold0%d.tex" and scaleTex[i] from "scale0%d.tex"
//     (both through sprintf, looked up in inventoryLump).
//   * single names, all in inventoryLump: invHeader "inv_header.tex"; lcursor
//     "cursorleft.tex"; rcursor "cursorright.tex"; marble "bigback.tex"; bevel
//     "marblebevel.tex"; stage "stage.tex"; selectionBox[0..3] "striptop.tex",
//     "stripbottom.tex", "stripright.tex", "stripleft.tex"; taskBar[0..11]
//     "tabweapon", "tabarmor", "tabmagic", "tabstats", "tabquests", "tababilities",
//     then the same six with "glow" before ".tex"; arrowLeft "arrowleft.tex";
//     arrowRight "arrowright.tex".
//   * i = 0..8: tinyIcon[i] from "tiny%s.tex" with iconName[i] (a table of
//     strings).
//   * Finally lcursor and rcursor are looked up and assigned a second time.
// The order of lumpFind calls is the order of the list above. The names of the
// destination words are the image's own symbols (iconName is the file-local table
// iconName.1132 there, so it is an unresolved import here); what some of them draw is unknown.
extern "C" {
extern void* inventoryLump;
extern void* iconLump;
extern void* hudLump;
extern void* moneyTex[7];
extern void* scaleTex[7];
extern void* invHeader;
extern void* lcursor;
extern void* rcursor;
extern void* marble;
extern void* bevel;
extern void* stage;
extern void* selectionBox[4];
extern void* taskBar[12];
extern void* arrowLeft;
extern void* arrowRight;
extern void* tinyIcon[9];
extern char* iconName[9];
void* lumpLoad__FPc(char*);
void** lumpFind__FPvPc(void*, char*);
int sprintf(char*, const char*, ...);

#define FIND(name) (lumpFind__FPvPc(inventoryLump, const_cast<char*>(name))[1])

void invInitSharedInterfaceTextures__Fv() {
    char name[128];
    inventoryLump = lumpLoad__FPc(const_cast<char*>("inventry.lmp"));
    iconLump = lumpLoad__FPc(const_cast<char*>("icons.lmp"));
    hudLump = lumpLoad__FPc(const_cast<char*>("hud.lmp"));
    for (int i = 0; i < 7; i++) {
        sprintf(name, "gold0%d.tex", i);
        moneyTex[i] = lumpFind__FPvPc(inventoryLump, name)[1];
    }
    for (int i = 0; i < 7; i++) {
        sprintf(name, "scale0%d.tex", i);
        scaleTex[i] = lumpFind__FPvPc(inventoryLump, name)[1];
    }
    invHeader = FIND("inv_header.tex");
    lcursor = FIND("cursorleft.tex");
    rcursor = FIND("cursorright.tex");
    marble = FIND("bigback.tex");
    bevel = FIND("marblebevel.tex");
    stage = FIND("stage.tex");
    selectionBox[0] = FIND("striptop.tex");
    selectionBox[1] = FIND("stripbottom.tex");
    selectionBox[2] = FIND("stripright.tex");
    selectionBox[3] = FIND("stripleft.tex");
    taskBar[0] = FIND("tabweapon.tex");
    taskBar[1] = FIND("tabarmor.tex");
    taskBar[2] = FIND("tabmagic.tex");
    taskBar[3] = FIND("tabstats.tex");
    taskBar[4] = FIND("tabquests.tex");
    taskBar[5] = FIND("tababilities.tex");
    taskBar[6] = FIND("tabweaponglow.tex");
    taskBar[7] = FIND("tabarmorglow.tex");
    taskBar[8] = FIND("tabmagicglow.tex");
    taskBar[9] = FIND("tabstatsglow.tex");
    taskBar[10] = FIND("tabquestsglow.tex");
    taskBar[11] = FIND("tababilitiesglow.tex");
    arrowLeft = FIND("arrowleft.tex");
    arrowRight = FIND("arrowright.tex");
    for (int i = 0; i < 9; i++) {
        sprintf(name, "tiny%s.tex", iconName[i]);
        tinyIcon[i] = lumpFind__FPvPc(inventoryLump, name)[1];
    }
    lcursor = FIND("cursorleft.tex");
    rcursor = FIND("cursorright.tex");
}
#undef FIND
}
