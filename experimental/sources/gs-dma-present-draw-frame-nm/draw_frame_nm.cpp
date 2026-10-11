// Observed drawFrameNm at 0x004844a8. Four .sdata bytes, file value 1024.
extern "C" unsigned drawFrameNm = 1024u;
static_assert(sizeof(drawFrameNm) == 4, "one word");
