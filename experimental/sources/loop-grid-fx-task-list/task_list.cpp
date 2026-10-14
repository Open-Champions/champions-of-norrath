// Observed taskList at 0x793e60, local BSS, symbol size 0. The next label,
// colorStack, is 512 bytes later, so this object is given that distance: room
// for 32 sixteen-byte task records (callback, key, text, flags). The distance
// is an upper bound for the storage; the image does not record a count. The
// bytes start as zero.
extern "C" {
unsigned char taskList[512];
}
static_assert(sizeof taskList == 512u, "taskList extent");
