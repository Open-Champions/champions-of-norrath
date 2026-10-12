// Observed lightHaloTexture at 0x004869dc. The ELF size is 0. menuPlayerNm
// begins four bytes later, so this slot is four bytes. It is .sbss and
// starts at zero. worldSetLightHaloTexture stores one pointer here.
extern "C" void* lightHaloTexture = nullptr;
