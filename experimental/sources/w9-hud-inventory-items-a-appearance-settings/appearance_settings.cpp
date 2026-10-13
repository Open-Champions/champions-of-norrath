// Observed in the pinned SLUS_205.65: four consecutive local zero-initialised byte
// arrays in BSS, 16 bytes apart: hairStyleSetting 0x008d5b38, hairColorSetting
// 0x008d5b48, beardSetting 0x008d5b58, skinSetting 0x008d5b68. The image gives them no
// ELF size; 16 bytes is the spacing between them, and the changeHairStyle,
// changeHairColor, changeBeard and changeSkin functions index them with a player
// number 0..9 (from the select screen caller).
// Inferred: signed byte elements (each is loaded with a signed byte load).
// Unknown: whether the true extent is 16, and what other code touches them.
extern "C" {
signed char hairStyleSetting[16];
signed char hairColorSetting[16];
signed char beardSetting[16];
signed char skinSetting[16];
}
