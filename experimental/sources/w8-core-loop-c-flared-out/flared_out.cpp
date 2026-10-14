// flaredOut at 0x00486b58, 4 bytes, in SLUS_205.65.
// Observed: local word in BSS, accessed with word loads and stores by dialogMsgCallback, drawDialog and runDialog. Unknown: its meaning.
extern "C" {
int flaredOut = 0;
}
