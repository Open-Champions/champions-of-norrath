extern "C" int norrath_vi_language_default __asm__("_10VILanguage$Default") = 9;
#include "language_set.cpp"

int main() {
    if (norrath_vi_language_default != 9) return 1;
    SetDefault__10VILanguage12VILanguageID(0);
    if (norrath_vi_language_default != 0) return 2;
    SetDefault__10VILanguage12VILanguageID(3);
    if (norrath_vi_language_default != 3) return 3;
    SetDefault__10VILanguage12VILanguageID(-1);
    if (norrath_vi_language_default != -1) return 4;
    return 0;
}
