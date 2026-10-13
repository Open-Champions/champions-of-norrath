// Observed, all four-byte-aligned words:
//   moneyTex  0x00427268  28 bytes in .data, all zero: seven texture pointers
//             (invInitSharedInterfaceTextures__Fv fills moneyTex[0..6] from the
//             lump entries "gold0%d.tex").
//   lcursor   0x004848e4  4 bytes in .data, zero: a texture pointer, set by the same
//             routine from "cursorleft.tex" and drawn by the shop menu.
//   iconLump  0x00486a80  4 bytes in .bss: the pointer returned for "icons.lmp".
//   hudLump   0x00486a84  4 bytes in .bss: the pointer returned for "hud.lmp".
// Inferred: the pointer types, from those uses only.
// Unknown: what moneyTex[i] shows for each i.
extern "C" {
void* moneyTex[7] = {};
void* lcursor = 0;
void* iconLump;
void* hudLump;
}
