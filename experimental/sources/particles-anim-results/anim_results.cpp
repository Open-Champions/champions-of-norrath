// animResults at 0x006a3d40, 156800 bytes, to identityResult.
// 100 records of 1568. BSS is zero. animInit fills the links later.
extern "C" {
unsigned char animResults[100 * 1568];
}
static_assert(sizeof animResults == 156800u, "animResults extent");
