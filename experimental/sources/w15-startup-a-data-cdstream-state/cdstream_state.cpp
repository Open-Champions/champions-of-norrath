// Observed: state shared by the CD-stream request functions (0x01000958 to 0x01000b60) and
// FlushIop__Fi (0x01000b68), from the pinned SLUS_205.65 symbol table:
//   sSend     0x011e25c0  request block; the next local symbol, sRecv, is 0x40 bytes later
//   sRecv     0x011e2600  reply block; the next symbol, sPending, is 0x40 bytes later
//   sPending  0x011e2640  one word
//   sStatus   0x011e2644  one word
//   last_buffer 0x0118e544  a global word (the buffer argument of the last read request)
// All read as zero in the image. The local names sSend, sRecv and sStatus are shared by other
// unrelated local objects, so the first four carry the norrath_cdstream_ prefix here and are
// not the image's names; last_buffer is the image's own global name.
// Inferred: sizes are the gaps between symbols (upper bounds); relative placement is not
// reproduced.
// Unknown: the true extent of the request and reply blocks and the readers of last_buffer.
extern "C" {
alignas(64) int norrath_cdstream_send[16];
alignas(64) int norrath_cdstream_recv[16];
int norrath_cdstream_pending = 0;
int norrath_cdstream_status = 0;
void* last_buffer = nullptr;
}
