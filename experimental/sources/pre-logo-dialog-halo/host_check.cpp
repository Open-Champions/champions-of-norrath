// Host contract for lightHaloTexture. Not an EE plan input.
#include "light_halo.cpp"

int main() {
    if (lightHaloTexture != nullptr) return 1;
    char object;
    lightHaloTexture = &object;
    if (lightHaloTexture != &object) return 2;
    return 0;
}
