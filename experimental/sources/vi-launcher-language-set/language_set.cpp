// Observed SetDefault__10VILanguage12VILanguageID at 0x010662c0, 12 bytes.
// It stores its argument in _10VILanguage$Default at 0x0118eeb8.
// GetDefault defines that word. This file only stores it.
extern "C" {
extern int norrath_vi_language_default __asm__("_10VILanguage$Default");

void SetDefault__10VILanguage12VILanguageID(int value) {
    norrath_vi_language_default = value;
}
}
