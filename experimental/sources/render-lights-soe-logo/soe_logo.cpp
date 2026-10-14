// Observed displaySOELogo__Fv at 0x2cd7d8, extent 188.
// It finds soe_splash_url.tex and soe_splash.tex in soelogo.lmp. The first
// draw arguments are 192, 330, 3 and the second are 64, 50, 3. Both colors
// are 0x80808080. The return gate matches displaySBLogoScreen: frameNm at
// 0x4844a4, the words at pad+100 and pad+12, and skipIntro at 0x4866c0.
// This address is not an object already authored on this branch.

extern "C" char* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" void textureDrawColor__FP8_textureiiiUi(char* texture, int x, int y, int mode, unsigned color);
extern "C" unsigned char pad[];
extern "C" int frameNm;
extern "C" int skipIntro;

static unsigned load_word(const unsigned char* bytes) {
    return unsigned(bytes[0]) | (unsigned(bytes[1]) << 8) |
           (unsigned(bytes[2]) << 16) | (unsigned(bytes[3]) << 24);
}

extern "C" int displaySOELogo__Fv() {
    char* lump = const_cast<char*>("soelogo.lmp");
    char* url = lumpFindResource__FPcT0(lump, const_cast<char*>("soe_splash_url.tex"));
    textureDrawColor__FP8_textureiiiUi(url, 192, 330, 3, 0x80808080u);
    char* splash = lumpFindResource__FPcT0(lump, const_cast<char*>("soe_splash.tex"));
    textureDrawColor__FP8_textureiiiUi(splash, 64, 50, 3, 0x80808080u);
    if (frameNm >= 301) return 1;
    unsigned at100 = load_word(pad + 100);
    unsigned at12 = load_word(pad + 12);
    if ((at100 | at12) != 0u) {
        skipIntro = 1;
        return 1;
    }
    return 0;
}
