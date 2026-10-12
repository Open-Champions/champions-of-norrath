// Observed per-player state of the yes/no box, four entries each:
//   yesNoPos    0x007d08d0, 16 bytes, zero-initialised (a local object in
//               the original unit; 0 or 1 is the highlighted choice).
//   activeDelay 0x004282a0, 16 bytes, initialised to four words of 3 (also
//               a local object; the input-blocking countdown).
// Both are read and written only through hudRunYesNo__FPUsiiii in the
// authored sources. Unknown: whether other functions of the original unit
// touch them. Here they are ordinary globals under the original names.
extern "C" {
int yesNoPos[4];
int activeDelay[4] = {3, 3, 3, 3};
}
