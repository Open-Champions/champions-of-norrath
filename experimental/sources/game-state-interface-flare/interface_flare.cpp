// Observed coreInterfaceFlare__Fi at 0x002d0c90, 28 bytes.
// The float bits 0x40000000 are stored at coreInterfaceBrightness plus the
// argument shifted left by 2. coreInterfaceBrightness is 16 bytes at
// 0x004447a8. The shift is the index of a 4-byte slot. There is no range
// test in the body. The caller in the pause-menu check passes 0. Nothing
// here was executed.
extern "C" float coreInterfaceBrightness[4];

extern "C" void coreInterfaceFlare__Fi(int index) {
    coreInterfaceBrightness[index] = 2.0f;
}
