// Observed worldSetLightHaloTexture__FP8_texture at 0x0017dd68, 8 bytes.
// It stores the argument in the word at lightHaloTexture. menuPlayerNm
// begins four bytes later. The ELF size is 0. lightHaloTexture is not
// defined here.
extern "C" void* lightHaloTexture;

extern "C" void worldSetLightHaloTexture__FP8_texture(void* texture) {
    lightHaloTexture = texture;
}
