// Observed hairStyleSetting at 0x008d5b38, 16 bytes, section .bss (local b).
// A local uninitialised object with no ELF size. The next symbol, hairColorSetting, starts 16 bytes later; hairColorSetting, beardSetting and skinSetting follow at 16-byte spacing. changeHairStyle indexes it with a player number 0 to 9 using signed byte loads.
// Inferred: 16 signed bytes (the spacing is taken as the size), zero-initialised here.
// Unknown: Whether the true extent is 16 and what else touches it.
extern "C" {
signed char hairStyleSetting[16];
}
