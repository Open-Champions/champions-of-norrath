// psSequenceNm at 0x00484308, four .sdata bytes, next currentPS.
// The file word is 01 00 00 00. initParticleSystems stores 1 later.
// The reviewed -G0 profile does not place this word in .sdata.
extern "C" {
unsigned psSequenceNm = 1u;
}
static_assert(sizeof psSequenceNm == 4u, "psSequenceNm width");
