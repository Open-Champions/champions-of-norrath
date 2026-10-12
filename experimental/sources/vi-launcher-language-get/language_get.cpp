// Observed GetDefault__10VILanguage at 0x010662b0, 12 bytes. It returns
// the word _10VILanguage$Default at 0x0118eeb8. The file bytes are zero.
extern "C" {
int norrath_vi_language_default __asm__("_10VILanguage$Default") = 0;

int GetDefault__10VILanguage() {
    return norrath_vi_language_default;
}
}
