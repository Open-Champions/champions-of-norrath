// Host check for invInitSharedInterfaceTextures__Fv. Not an EE plan input.
#include "inv_init_shared_textures.cpp"
#include <cstdio>
#include <string>
#include <vector>
void *inventoryLump, *iconLump, *hudLump, *moneyTex[7], *scaleTex[7], *invHeader, *lcursor, *rcursor, *marble, *bevel, *stage,
    *selectionBox[4], *taskBar[12], *arrowLeft, *arrowRight, *tinyIcon[9];
char* iconName[9];
static std::vector<std::string> calls;
static void* table[2];
extern "C" {
void* lumpLoad__FPc(char* n) { calls.push_back(std::string("L:") + n); return reinterpret_cast<void*>(calls.size() * 16); }
void** lumpFind__FPvPc(void* l, char* n) {
    calls.push_back(std::string("F:") + n);
    static void* rec[64][2];
    void** r = rec[calls.size()];
    r[1] = reinterpret_cast<void*>(0x1000 + calls.size());
    return r;
}
}
int main() {
    static char names[9][8];
    for (int i = 0; i < 9; i++) { snprintf(names[i], 8, "n%d", i); iconName[i] = names[i]; }
    invInitSharedInterfaceTextures__Fv();
    if (calls.size() != 3 + 7 + 7 + 6 + 4 + 12 + 2 + 9 + 2) return 1;
    if (calls[0] != "L:inventry.lmp" || calls[1] != "L:icons.lmp" || calls[2] != "L:hud.lmp") return 2;
    if (calls[3] != "F:gold00.tex" || calls[9] != "F:gold06.tex" || calls[10] != "F:scale00.tex" || calls[16] != "F:scale06.tex") return 3;
    if (calls[17] != "F:inv_header.tex" || calls[22] != "F:stage.tex" || calls[23] != "F:striptop.tex" || calls[27] != "F:tabweapon.tex") return 4;
    if (calls[38] != "F:tababilitiesglow.tex" || calls[39] != "F:arrowleft.tex" || calls[41] != "F:tiny" "n0.tex") return 5;
    if (calls[49] != "F:tinyn8.tex" || calls[50] != "F:cursorleft.tex" || calls[51] != "F:cursorright.tex") return 6;
    if (iconLump != reinterpret_cast<void*>(2 * 16) || hudLump != reinterpret_cast<void*>(3 * 16)) return 7;
    if (invHeader != reinterpret_cast<void*>(0x1000 + 18) || taskBar[11] != reinterpret_cast<void*>(0x1000 + 39)) return 8;
    // the second cursor lookups overwrite the first ones
    if (lcursor != reinterpret_cast<void*>(0x1000 + 51) || rcursor != reinterpret_cast<void*>(0x1000 + 52)) return 9;
    if (tinyIcon[8] != reinterpret_cast<void*>(0x1000 + 50)) return 10;
    return 0;
}
