// Observed crystalTex at 0x004286b0, 16 bytes in .data, file bytes all zero. The authored HUD
// initialisation stores four texture pointers into it ("playerred.tex", "playergreen.tex",
// "playeryellow.tex" and "playerblue.tex"), so it is defined here as four pointer words. The image
// places it in .data between quickSpellMenu and drawBustTexName; this source gives it the
// reviewed zero-initialised storage (BSS in the relocatable object), so the placement is not
// reproduced. Nothing here was executed on the EE.
extern "C" {
char* crystalTex[4];
}
