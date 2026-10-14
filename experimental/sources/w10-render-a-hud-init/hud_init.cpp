// Observed HUDInit__Fv at 0x001c40c0, 208 bytes. It registers two engine tasks:
// HUDRunFrame__Fv (0x001c3bc0) with priority 50, the name "HUD Run" and flag 0, then
// HUDDrawFrame__Fv (0x001c3ec0) with priority 51, the name "HUD Draw" and flag 1. It then
// looks up four resources in the lump named "hud.lmp" with lumpFindResource and stores
// the results in the four words of crystalTex (0x004286b0): "playerred.tex",
// "playergreen.tex", "playeryellow.tex" and "playerblue.tex" in that order. Last it
// stores 1.0f in the float bustAlpha (0x00484b08) and zero in the bytes hudBustCovered
// (0x00484af9) and hudNetWaiting (0x00484b00). Nothing is returned.
// Inferred: the four textures are per-player marker images; bustAlpha is the fade of the
// bust portrait. Unknown: the meaning of the task priorities, and whether the lump is
// loaded before this call (the image does not check the lookups for null).
extern "C" {
void HUDRunFrame__Fv();
void HUDDrawFrame__Fv();
void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);
char* lumpFindResource__FPcT0(char* lump, char* name);
extern char* crystalTex[4];
extern float bustAlpha;
extern unsigned char hudBustCovered;
extern unsigned char hudNetWaiting;

void HUDInit__Fv() {
    engineAddTask__FPFv_viPci(HUDRunFrame__Fv, 50, const_cast<char*>("HUD Run"), 0);
    engineAddTask__FPFv_viPci(HUDDrawFrame__Fv, 51, const_cast<char*>("HUD Draw"), 1);
    char* lump = const_cast<char*>("hud.lmp");
    crystalTex[0] = lumpFindResource__FPcT0(lump, const_cast<char*>("playerred.tex"));
    crystalTex[1] = lumpFindResource__FPcT0(lump, const_cast<char*>("playergreen.tex"));
    crystalTex[2] = lumpFindResource__FPcT0(lump, const_cast<char*>("playeryellow.tex"));
    crystalTex[3] = lumpFindResource__FPcT0(lump, const_cast<char*>("playerblue.tex"));
    bustAlpha = 1.0f;
    hudBustCovered = 0;
    hudNetWaiting = 0;
}
}
