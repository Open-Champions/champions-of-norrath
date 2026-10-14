// Observed: EnableCr__9VIWndEditi at 0x010ad560 is 40 bytes. It stores its argument at this+440,
// then writes the byte 6 into byte 64 of the 66-byte table sKeyMap (0x01192b80) when the argument is
// nonzero and 0 when it is zero. Returns nothing. IsCrEnabled__9VIWndEdit (0x010ad588) is the
// matching reader of the word at this+440.
// Inferred: the byte at sKeyMap+64 is the entry the edit switches on for the Enter key.
// Unknown: the layout of sKeyMap, which is a file-local symbol of the image, so here it is an
// import with that name and size only.
extern "C" {
extern unsigned char sKeyMap[66];

void EnableCr__9VIWndEditi(void* self, int enable) {
    *reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 440) = enable;
    sKeyMap[64] = enable != 0 ? 6 : 0;
}
}
