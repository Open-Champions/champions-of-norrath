// Observed displaySBLogoScreen__Fv at 0x2d0cc8, extent 128.
// It finds snowlogo.tex in sblogo.lmp and draws that texture at arguments
// 0, 0, 3 with color 0x80808080. The symbol name does not encode a return
// type. The body returns 1 when frameNm is not below 301. Below that, a
// nonzero word at pad+100 or pad+12 stores 1 to skipIntro and returns 1;
// both words zero returns 0 and leaves skipIntro unchanged. pad is the
// 352-byte object at 0x34beb8. frameNm is the 4-byte word at 0x4844a4.
// skipIntro is the 4-byte word at 0x4866c0. This address is not an object
// already authored on this branch.

extern "C" char* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" void textureDrawColor__FP8_textureiiiUi(char* texture, int x, int y, int mode, unsigned color);
extern "C" unsigned char pad[];
extern "C" int frameNm;
extern "C" int skipIntro;

static unsigned load_word(const unsigned char* bytes) {
    return unsigned(bytes[0]) | (unsigned(bytes[1]) << 8) |
           (unsigned(bytes[2]) << 16) | (unsigned(bytes[3]) << 24);
}

extern "C" int displaySBLogoScreen__Fv() {
    char* texture = lumpFindResource__FPcT0(
        const_cast<char*>("sblogo.lmp"), const_cast<char*>("snowlogo.tex"));
    textureDrawColor__FP8_textureiiiUi(texture, 0, 0, 3, 0x80808080u);
    if (frameNm >= 301) return 1;
    unsigned at100 = load_word(pad + 100);
    unsigned at12 = load_word(pad + 12);
    if ((at100 | at12) != 0u) {
        skipIntro = 1;
        return 1;
    }
    return 0;
}
