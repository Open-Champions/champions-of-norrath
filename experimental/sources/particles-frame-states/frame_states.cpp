// frameStates at 0x006162c0, 580224 bytes, to animResults.
// 150 records of 3868, plus 24 bytes the anim clear does not write.
// BSS is zero. animInit fills the links later.
extern "C" {
unsigned char frameStates[150 * 3868 + 24];
}
static_assert(sizeof frameStates == 580224u, "frameStates extent");
