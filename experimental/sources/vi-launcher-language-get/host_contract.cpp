#include "language_get.cpp"

int main() {
    if (GetDefault__10VILanguage() != 0) return 1;
    norrath_vi_language_default = 4;
    if (GetDefault__10VILanguage() != 4) return 2;
    norrath_vi_language_default = -7;
    if (GetDefault__10VILanguage() != -7) return 3;
    return 0;
}
