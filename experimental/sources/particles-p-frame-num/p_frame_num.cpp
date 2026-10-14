// P_FrameNum at 0x004869f4, two .sbss bytes, next P_SaveFrameNum.
// The image value is zero. P_Init stores zero here later.
extern "C" {
unsigned short P_FrameNum;
}
static_assert(sizeof P_FrameNum == 2u, "P_FrameNum width");
