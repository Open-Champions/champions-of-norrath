// trailSequenceNm at 0x00484310. The symbol size is 4.
// cutMode is at 0x00484318, so four file bytes between them are not this word.
// The file word is 01 00 00 00. initTrails stores 1 later.
// The reviewed -G0 profile does not place this word in .sdata.
extern "C" {
unsigned trailSequenceNm = 1u;
}
static_assert(sizeof trailSequenceNm == 4u, "trailSequenceNm width");
