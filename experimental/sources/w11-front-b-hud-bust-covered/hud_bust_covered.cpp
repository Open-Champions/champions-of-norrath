// Observed hudBustCovered at 0x00484af9, one byte in .data, zero in the image. HUDInit
// stores zero into it. The neighbouring word bustAlpha is at 0x00484b08.
// Unknown: what it records; the name suggests the bust portrait being covered, which
// is not shown by any code read so far.
extern "C" {
unsigned char hudBustCovered = 0;
}
