// Observed beardSetting at 0x008d5b58, 16 bytes, section .bss (local b).
// A local uninitialised object with no ELF size, 16 bytes before skinSetting and after hairColorSetting. changeBeard indexes it with a player number 0 to 9 using signed byte loads.
// Inferred: 16 signed bytes (the spacing is taken as the size), zero-initialised here.
// Unknown: Whether the true extent is 16 and what else touches it.
extern "C" {
signed char beardSetting[16];
}
