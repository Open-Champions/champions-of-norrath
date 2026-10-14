// Observed: actAlignData at 0x00484058 is 6 bytes of initialized data: 00 01 ff ff ff ff.
// actSendData at 0x00486930 is an 8-byte object in uninitialized storage (the next symbols are
// needActAlign at 0x00486928 and scx1 at 0x00486938). The only reader in the authored sources
// declares them as byte arrays of those lengths.
// Inferred: nothing.
// Unknown: what the bytes mean; whether actSendData is a buffer or a pair of words.

extern "C" {
unsigned char actAlignData[6] = {0x00, 0x01, 0xff, 0xff, 0xff, 0xff};
unsigned char actSendData[8];
}
