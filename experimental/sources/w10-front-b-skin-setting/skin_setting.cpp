// Observed in the pinned SLUS_205.65 image: skinSetting at 0x008d5b68 is a local zero-initialised
// .bss object with no ELF size; the next symbol, yaws, is 16 bytes later. changeSkin indexes it by
// player number and loads and stores signed bytes.
// The earlier unit w9-hud-inventory-items-a-appearance-settings defines the same symbol together
// with three siblings and is not selected; this unit defines skinSetting alone with a check.
// Unknown: whether more than four entries are used.
extern "C" {
signed char skinSetting[16];
}
