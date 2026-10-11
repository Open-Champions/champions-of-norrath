#include <cstring>
#include <cstdint>

extern "C" unsigned char controlStyle;
extern "C" unsigned dropButton;
extern "C" unsigned dropButtonFontChar;
extern "C" unsigned equipButton;
extern "C" unsigned equipButtonFontChar;
extern "C" unsigned infoButton;
extern "C" unsigned infoButtonFontChar;
extern "C" unsigned padAcceptButton;
extern "C" unsigned padAcceptButtonFontChar;
extern "C" unsigned padBackButton;
extern "C" unsigned padBackButtonFontChar;
extern "C" unsigned padVibrationEnable;

static int bytes(const void* object, const unsigned char* want, unsigned size) {
    return std::memcmp(object, want, size) == 0 ? 0 : 1;
}

int main() {
    const unsigned char one[] = {0x01};
    const unsigned char zero[] = {0x00, 0x00, 0x00, 0x00};
    const unsigned char accept[] = {0x40, 0x00, 0x00, 0x00};
    const unsigned char back[] = {0x20, 0x01, 0x00, 0x00};
    const unsigned char vibe[] = {0x01, 0x00, 0x00, 0x00};
    if (sizeof controlStyle != 1 || bytes(&controlStyle, one, 1)) return 1;
    if (bytes(&dropButton, zero, 4) || bytes(&dropButtonFontChar, zero, 4)) return 2;
    if (bytes(&equipButton, zero, 4) || bytes(&equipButtonFontChar, zero, 4)) return 3;
    if (bytes(&infoButton, zero, 4) || bytes(&infoButtonFontChar, zero, 4)) return 4;
    if (bytes(&padAcceptButton, accept, 4) || bytes(&padAcceptButtonFontChar, zero, 4)) return 5;
    if (bytes(&padBackButton, back, 4) || bytes(&padBackButtonFontChar, zero, 4)) return 6;
    if (bytes(&padVibrationEnable, vibe, 4)) return 7;
    if (padAcceptButton != 0x40u || padBackButton != 0x120u) return 8;
    return 0;
}
