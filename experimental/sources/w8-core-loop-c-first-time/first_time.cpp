// firstTime at 0x00486d88, 1 bytes, in SLUS_205.65.
// Observed: local one-byte BSS object read and written with byte accesses by playerSelectionStart and playerSelectionTask. A second local object also named firstTime exists at 0x00483e40 (one byte, initial 1, used by msg_run of the WaveMaker class); that one is a different symbol and is not defined here. Unknown: whether any unit expects the second one under this name.
extern "C" {
unsigned char firstTime = 0;
}
