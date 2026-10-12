// Observed: initPreSelect__Fv at 0x00280610 is 80 bytes (a local, non-exported
// function). It stores the single-precision constant 0.054 less one unit in the last place (the word at
// 0x00482320, 0x3d5d2f1a) in the float at 0x00486d38, zero in the word at
// 0x00486d3c, zero in lockAlpha and zero in selectMode, then calls
// initPosition__Fii(0, b) where b is the signed byte at offset 32 of record
// curPlayer in playerSelectInfo; records are 41 bytes apart. It returns nothing.
// Inferred: the float at 0x00486d38 is the symbol the image calls "delta"; the
// word at 0x00486d3c is the file-local "flareAlpha" (the image holds several
// objects named flareAlpha and flareDelta, so only this address pairing ties
// them to this routine). The 41-byte stride is inferred from the scaling
// arithmetic (curPlayer * 5 * 8 + curPlayer).
// Unknown: the meaning of the byte at offset 32 of a playerSelectInfo record,
// and whether the two statics are the ones other player-select code reads.
// Note for the linker: this unit names the file-local objects with their image
// names; they must not be bound to the other objects called flareAlpha.

extern "C" void initPosition__Fii(int a, int b);
extern "C" int curPlayer;
extern "C" int selectMode;
extern "C" int lockAlpha;
extern "C" int flareAlpha;
extern "C" float delta;
extern "C" signed char playerSelectInfo[];

extern "C" void initPreSelect__Fv() {
    delta = 0x1.ba5e34p-5f;
    flareAlpha = 0;
    lockAlpha = 0;
    selectMode = 0;
    initPosition__Fii(0, playerSelectInfo[32 + 41 * curPlayer]);
}
