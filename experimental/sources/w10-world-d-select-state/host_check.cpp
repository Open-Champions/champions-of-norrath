// Host contract for the selection words and constants. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
extern char* selectLumpName;
extern int selectMode, thisLevel, wasFormatting;
extern void* stage;
extern void (*userFilterFunction)(void*);
extern const float scale[3];
}
int main() {
    if (std::strcmp(selectLumpName, "select.lmp") != 0) return 1;
    if (selectMode != 0 || thisLevel != 0 || wasFormatting != 0 || stage != nullptr || userFilterFunction != nullptr) return 2;
    unsigned bits[3];
    std::memcpy(bits, scale, sizeof bits);
    if (bits[0] != 0x3f666666u || bits[1] != 0x3f199999u || bits[2] != 0) return 3;
    thisLevel = 4;
    if (thisLevel != 4) return 4;
    std::printf("select state contract passed\n");
    return 0;
}
