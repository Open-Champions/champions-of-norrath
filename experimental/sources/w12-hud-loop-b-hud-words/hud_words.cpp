// Observed in the pinned SLUS_205.65 image: five small HUD-area objects whose ELF symbols carry no
// size (or whose size is a single word). Every one starts as zero bytes.
//   levelUpFade        .bss    0x007d06d8  16 bytes (multiTitle follows at +16)
//   multiBody          .bss    0x007d06f8  16 bytes (multiSide follows at +16)
//   multiHelpMessageOn .sdata  0x00483f38   4 bytes (playerRecalled follows at +4)
//   shipOn             .sdata  0x00484520   4 bytes (healColor follows at +8; +4 is unnamed padding)
//   shopPromptOK       .sdata  0x00484a20   2 bytes (gold follows at +4)
// Observed uses: HUDRunFrame indexes levelUpFade as an int per player and stores 0; the multi-player
// help routines index multiBody with a pointer per player and multiHelpMessageOn as a byte per
// player; the player draw routine reads shipOn as an int; the helpful-message routine indexes
// shopPromptOK by screen side 0/1 as bytes.
// Inferred: levelUpFade and multiBody hold one slot per player (four slots fill the observed gaps).
// Unknown: whether anything writes a nonzero shipOn; the meaning of the levelUpFade values.
// The reviewed -G0 profile keeps zero objects in ordinary BSS, so the small-data placement is not
// reproduced here.
extern "C" {
int levelUpFade[4];
unsigned short* multiBody[4];
unsigned char multiHelpMessageOn[4];
int shipOn;
unsigned char shopPromptOK[2];
}
