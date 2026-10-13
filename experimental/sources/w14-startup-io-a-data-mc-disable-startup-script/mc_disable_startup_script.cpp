// Observed: mcDisableStartupScript is a 1-byte object at 0x004859e0 in initialized data; the
// image byte is zero. MC_LoadLevelEntities tests it as a signed byte through the global pointer
// before running the "preStart" and "start" scripts.
// Inferred: a flag that suppresses those two script runs when nonzero.
// Unknown: which routine sets it, if any.
extern "C" {
signed char mcDisableStartupScript = 0;
}
