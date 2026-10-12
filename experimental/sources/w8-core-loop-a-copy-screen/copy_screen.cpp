// Observed copyScreen at 0x00791060 in zero-filled storage, 16-byte aligned. makeSamplePacket__Fb builds a DMA
// packet in it and writes at least 9232 bytes (1154 doublewords); doSuperSample__Fiib passes its address to
// sceDmaSend. The next symbol, frameStatePacket, begins 0x2800 (10240) bytes later; the ELF gives copyScreen
// no size, so 10240 is the gap and not a measured object size.
// Inferred: the object fits within the gap. Its declared element type is not known.
// Unknown: the real array length and the source type.
extern "C" {
alignas(16) unsigned char copyScreen[10240];
}
