// descListMark at 0x00780280, 128 bytes, to descList.
// 32 words. BSS is zero. P_Init later fills every byte with 0xff.
extern "C" {
unsigned char descListMark[128];
}
static_assert(sizeof descListMark == 128u, "descListMark extent");
