/* Observed engineSetVideoMode__Fb at 0x001913a0, 52 bytes. GP is 0x004879f0.
 * A nonzero engineDisable30fps (0x0048441c) returns. compositeAlpha
 * (0x00484428) compared signed with 128 returns when it is smaller.
 * fullFrameMode (0x004843e4) equal to the argument returns. Otherwise the
 * argument is stored in engineFullFrameMode (0x00484474) and fullFrameMode,
 * and the argument xor 1 is stored in trippleBufferMode (0x004843e0). The
 * spelling trippleBufferMode is the symbol name. Fresh compilation is not
 * an original-byte match.
 */
extern "C" int engineDisable30fps;
extern "C" int compositeAlpha;
extern "C" int fullFrameMode;
extern "C" int engineFullFrameMode;
extern "C" int trippleBufferMode;

extern "C" void engineSetVideoMode__Fb(int mode) {
    if (engineDisable30fps != 0) return;
    if (compositeAlpha < 128) return;
    if (fullFrameMode == mode) return;
    engineFullFrameMode = mode;
    fullFrameMode = mode;
    trippleBufferMode = mode ^ 1;
}
