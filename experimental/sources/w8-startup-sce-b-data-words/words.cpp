// Observed startup, CD and audio words. File-backed values are the image bytes.
// BSS objects are zero in the file and stay zero here. Sizes are the symbol
// sizes, or the span to the next symbol when the ELF size is zero. Alignments
// are the observed address alignments, not recovered attributes.
//
//   _sceCd_scmdsdata     0x004476c0  1032 bytes (.data, all zero)
//   eenull               0x00445940    40 bytes (.data, 20 bytes then 20 zero bytes)
//   batchBuffer          0x007dc280  4096 bytes (span to streamAllocation)
//   gCd                  0x007dd7e0    96 bytes (span to sbuff; the SIF client)
//   cancelStreamWait     0x00486b3c     4 bytes
//   decompressThreadid   0x00486968     4 bytes
//   dmaGIF               0x0118e6a4     4 bytes (overlay range)
//
// eenull holds five nonzero 32-bit machine-code words that InitAlarm copies to
// 0x00082000 as the null EE thread entry. They are kept as data words; this file
// does not claim to know what the program does.
extern "C" {
alignas(4) unsigned char _sceCd_scmdsdata[0x408] = {};
alignas(8) unsigned eenull[10] = {0x3c1d0008u, 0x0060f809u, 0x27bd1fc0u, 0x2403fff8u, 0x0000000cu};
alignas(64) unsigned char batchBuffer[0x1000] = {};
alignas(32) unsigned char gCd[0x60] = {};
int cancelStreamWait = 0;
int decompressThreadid = 0;
void* dmaGIF = 0;
}
