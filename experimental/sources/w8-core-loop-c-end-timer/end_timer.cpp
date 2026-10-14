// endTimer at 0x00484bec, 4 bytes, in SLUS_205.65.
// Observed: local word in .data with file value 1, accessed by runFrontEnd and stopCredits with word loads and stores. Neighbours curFocus, endInit and others are different objects.
extern "C" {
int endTimer = 1;
}
