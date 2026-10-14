// Observed storage in the pinned SLUS_205.65 image (symbol sizes from the ELF
// symbol table). Initial bytes read from the file image:
//   trueNetFrameNm         0x00483844  4 bytes, initial 1
//   packetSendCycleLength  0x00483848  4 bytes, initial 6
//   packetSendCycleTime    0x0048384c  4 bytes, initial 0
//   sentStatePacketFrameNm 0x00483864  4 bytes, initial 0
// Inferred: all four are read and written as signed 32-bit counters by the
// network frame code (types follow the authored callers). Unknown: what unit
// the send cycle length is counted in; nothing here assigns a meaning to 6.
extern "C" {
int trueNetFrameNm = 1;
int packetSendCycleLength = 6;
int packetSendCycleTime;
int sentStatePacketFrameNm;
}
