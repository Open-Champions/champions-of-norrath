extern "C" {
void* lightHaloTexture;
void worldSetLightHaloTexture__FP8_texture(void* texture);
}

int main() {
    char object;
    lightHaloTexture = &object;
    worldSetLightHaloTexture__FP8_texture(0);
    if (lightHaloTexture != 0) return 1;
    worldSetLightHaloTexture__FP8_texture(&object);
    if (lightHaloTexture != &object) return 2;
    worldSetLightHaloTexture__FP8_texture(reinterpret_cast<void*>(static_cast<unsigned long>(0x004869dcu)));
    if (lightHaloTexture != reinterpret_cast<void*>(static_cast<unsigned long>(0x004869dcu))) return 3;
    return 0;
}
