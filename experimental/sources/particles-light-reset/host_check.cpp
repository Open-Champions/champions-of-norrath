// Host contract for lightResetOptionalParams__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" void lightResetOptionalParams__Fv();
extern "C" unsigned char scene[56] = {};
extern "C" unsigned char charAmbient[8] = {};
extern "C" unsigned char charDirect[8] = {};
extern "C" unsigned char charDirect2[8] = {};
extern "C" unsigned char charDirectDir[16] = {};
extern "C" unsigned char charDirectDir2[16] = {};
extern "C" float charScaleR = 0.f;
extern "C" float charScaleG = 0.f;
extern "C" float charScaleB = 0.f;
extern "C" int interfaceMode = 0;
extern "C" unsigned ambientOnly = 1;
extern "C" unsigned disableVolumetricLighting = 1;

namespace {
unsigned short half_at(const unsigned char* slot) {
    return static_cast<unsigned short>(slot[0] | (slot[1] << 8));
}

unsigned float_bits(float value) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}
}

int main() {
    for (int byte = 0; byte < 56; ++byte) scene[byte] = static_cast<unsigned char>(byte + 1);
    scene[36] = 11;
    scene[37] = 22;
    scene[38] = 33;
    scene[39] = 44;
    scene[40] = 55;
    scene[41] = 66;
    scene[42] = 77;
    scene[43] = 88;
    scene[48] = 10;
    scene[49] = 20;
    scene[50] = 100;
    std::memset(scene + 12, 0x21, 12);
    std::memset(scene + 24, 0x43, 12);
    interfaceMode = 15;
    ambientOnly = 8;
    disableVolumetricLighting = 9;
    lightResetOptionalParams__Fv();
    if (half_at(charDirect) != 11 || half_at(charDirect + 2) != 22 || half_at(charDirect + 4) != 33 ||
        half_at(charDirect + 6) != 44) {
        std::fprintf(stderr, "fail: charDirect\n");
        return 1;
    }
    if (half_at(charAmbient) != 55 || half_at(charAmbient + 2) != 66 || half_at(charAmbient + 4) != 77 ||
        half_at(charAmbient + 6) != 88) {
        std::fprintf(stderr, "fail: charAmbient\n");
        return 1;
    }
    if (half_at(charDirect2) != 8 || half_at(charDirect2 + 2) != 16 || half_at(charDirect2 + 4) != 80 ||
        half_at(charDirect2 + 6) != 44) {
        std::fprintf(stderr, "fail: charDirect2 %u %u %u\n", half_at(charDirect2), half_at(charDirect2 + 2),
                     half_at(charDirect2 + 4));
        return 1;
    }
    if (float_bits(charScaleR) != 0x3f800000u || float_bits(charScaleG) != 0x3f800000u ||
        float_bits(charScaleB) != 0x3f800000u) {
        std::fprintf(stderr, "fail: scales\n");
        return 1;
    }
    if (interfaceMode != 5 || ambientOnly != 0u || disableVolumetricLighting != 0u) {
        std::fprintf(stderr, "fail: modes\n");
        return 1;
    }
    for (int byte = 0; byte < 12; ++byte) {
        if (charDirectDir[byte] != 0x21 || charDirectDir2[byte] != 0x43) {
            std::fprintf(stderr, "fail: directions\n");
            return 1;
        }
    }
    if (charDirectDir[12] != 0 || charDirectDir2[12] != 0) {
        std::fprintf(stderr, "fail: direction tail\n");
        return 1;
    }
    interfaceMode = 9;
    lightResetOptionalParams__Fv();
    if (interfaceMode != 9) {
        std::fprintf(stderr, "fail: mode below ten\n");
        return 1;
    }
    interfaceMode = -4;
    lightResetOptionalParams__Fv();
    if (interfaceMode != -4) {
        std::fprintf(stderr, "fail: negative mode\n");
        return 1;
    }
    return 0;
}
