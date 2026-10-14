// Observed: MC_GameConfig at 0x0042e300 is 252 bytes, all zero, in .data.
// MC_SaveConfigSilent ORs its first argument into the first word.
// Inferred: 252 is 63 words. gameInit reads fixed offsets inside this object.
// Unknown: the meaning of the words this package does not write.
extern "C" {
unsigned char MC_GameConfig[252];
}
