// Observed: UnlockMipLevel__9VISurfacei at 0x0112c9b8 is 32 bytes in the pinned SLUS-20565 image. If the word at offset 28 of the
// surface equals the level argument, it stores -1 there and returns zero; otherwise it changes nothing and returns -1.
// Inferred: the word at +28 is the currently locked mip level (-1 means none) and the result is an int status.
// Unknown: the rest of the surface layout and whether callers check the status. Nothing here ran on the EE.
extern "C" {
int UnlockMipLevel__9VISurfacei(int* self, int level) {
    if (self[7] != level) return -1;
    self[7] = -1;
    return 0;
}
}
