// Observed layout in the image:
//   MCA_Result        0x00485bfc  4 bytes, initialized, value 0
//   MCA_DirContents   0x0042e400  0x1000 bytes, initialized, all zero
//   cardType          0x00485c14  4 bytes, initialized, value 0
//   askedForConfigLoad 0x00484c8c 4 bytes, initialized, value 0
//   configLumpLoaded  0x00484c84  1 byte, initialized, value 0
//   browserSlot       0x00486c48  4 bytes in uninitialized storage (next symbol at +4)
//   configFormatted   0x00486cc9  1 byte in uninitialized storage (next symbol pcTitle at +3)
//   configFileName    0x007f80d0  0x30 bytes in uninitialized storage (next symbol at 0x007f8100)
// The extents of the uninitialized objects follow from the gap to the next symbol; the authored
// readers declare browserSlot and askedForConfigLoad as int, configFormatted and configLumpLoaded
// as signed char and configFileName as a char array.
// Inferred: configFormatted is a single byte flag; its three-byte gap is padding.
// Unknown: the meaning of cardType values, and the use of the rest of the 0x1000-byte directory
// buffer beyond what the memory-card routines read.

extern "C" {
int MCA_Result = 0;
unsigned char MCA_DirContents[0x1000] = {};
int cardType = 0;
int askedForConfigLoad = 0;
signed char configLumpLoaded = 0;
int browserSlot;
signed char configFormatted;
char configFileName[0x30];
}
