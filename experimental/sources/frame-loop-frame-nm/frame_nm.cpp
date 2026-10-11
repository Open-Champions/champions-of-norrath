// Observed frameNm at 0x004844a4, 4 bytes in .sdata. The file word is 0.
// simpleDrawLoop stores 0 on entry and adds 1 after each pass.
extern "C" {
unsigned frameNm = 0u;
}
static_assert(sizeof(frameNm) == 4, "frameNm is one word");
