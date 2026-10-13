#include "multi_help_words.cpp"
#include <cstdio>

int main() {
    int bad = 0;
    bad |= sizeof(multiRelease) != 4;
    bad |= sizeof(multiTitle) / sizeof(multiTitle[0]) != 4 || sizeof(multiSide) != 16;
    for (int i = 0; i < 4; ++i) bad |= multiRelease[i] != 0 || multiTitle[i] != 0 || multiSide[i] != 0;
    std::printf(bad ? "FAIL\n" : "ok: four slots each and zero start\n");
    return bad;
}
