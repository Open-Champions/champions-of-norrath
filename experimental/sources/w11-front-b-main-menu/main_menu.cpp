// Observed mainMenu__Fii at 0x001b2cf0, 1296 bytes. This is the shop's two-item
// menu (shop mode 3, called from shopRun). Arguments: a screen index n and a
// player index p. S = shop + 116 * n (116-byte screen records, 0x007d01f8); the
// words used are +0 (left x), +8 (width), +24 (selected item), +36 (y offset),
// +60 (float), +76 (mode) and +84 (written). Let accept(p) be
// (word at netLocalPad + 88 * p + 12) & padAcceptButton. In this order:
//   1. shopDrawTaskBar__FP12screenStruct(S); updateCursor__FiiiiiP12screenStructi(
//      p, 0, 2, 0, 39, S, 1). S+84 takes the value of S+24.
//   2. The texture "buttontop.tex" of lump "shop.lmp" (lumpFindResource__FPcT0)
//      is drawn with textureDrawColor__FP8_textureiiiUi at (S.x + 66, 210, 0,
//      0x80808080) when S+24 is 0; otherwise "buttonbottom.tex" at y 248.
//   3. c = S.x + S.width / 2. fontStart(defaultFont, 0, 0x80323c3c, -1).
//   4. Item 0: fontColor__FUiii is 0x8049616a when S+24 is 0, else 0x80323c3c
//      (second argument -1, third 0); fontPrintCenteredu__FiiPUsi(c, 203,
//      textGetString("Buy"), 1000000). When selected and accept(p) is set and the
//      float at S+60 equals 0.0, resetMenu__FP12screenStruct(S) and S+76 = 0.
//   5. Item 1 the same way with S+24 re-read, y 243 and "Sell". When selected and
//      accept(p) with the float at S+60 equal to 0.0: inventoryUnpack__Fiib(p, n,
//      false), resetMenu(S), S+76 = 1.
//   6. When shopDisableMouse[n] and shopMouseDecay[n] are both zero and
//      (word at netLocalPad + 88 * p + 12) & padBackButton: if netPlayerIsLocal(p)
//      the animation "menu_shop_back.anm" of "fx.lmp" is played with
//      soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, 0, 1.0f); and
//      S+76 = 2. Then fontEnd.
//   7. Only when S+76 is 3: "buysell.tex" of "shop.lmp" is drawn at (S.x + 60,
//      174, 0, 0x80808080). For the signed byte `language`: 3 gives (s, a) =
//      (6, -5), 4 gives (5, -3), 1 gives (9, -3), anything else (0, 0). Then, with
//      the right cursor texture rc and the left one lc, A = (double)(a + 10),
//      B = (double)p * 1.5 and T = (double)((float)drawFrameNm * 0.09999999f)
//      (the float constant is the image word 0x3dcccccc): the offset
//      d = (long)(A * sin(B + T)) is evaluated twice. rc is drawn with its first
//      halfword w (read as its width) at (c - s - w - d - 38, S.y + 206, 1,
//      0x80808080) and lc at (c + d + s + 44, S.y + 206, 1, 0x80808080).
// Doubles go through the soft routines litodp, dpmul, fptodp, dpadd, sin and
// dptoli, which carry a double in one 64-bit register. Nothing returns a value.
// Unknown: what the fourth argument 39 and the others of updateCursor mean, why
// language values 1, 3 and 4 move the arrows, and what S+84 is for.
typedef unsigned long long Double;

extern "C" {
extern unsigned char shop[];
extern unsigned char netLocalPad[];
extern int padAcceptButton;
extern int padBackButton;
extern void* defaultFont;
extern signed char language;
extern void* rcursor;
extern void* lcursor;
extern int drawFrameNm;
extern int shopDisableMouse[2];
extern int shopMouseDecay[2];

void shopDrawTaskBar__FP12screenStruct(unsigned char* screen);
void updateCursor__FiiiiiP12screenStructi(int, int, int, int, int, unsigned char*, int);
void* lumpFindResource__FPcT0(char* lump, char* name);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int flags, unsigned color);
void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
void fontColor__FUiii(unsigned color, int a, int b);
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int width);
unsigned short* textGetString__FPc(char* key);
void fontEnd__Fv();
void resetMenu__FP12screenStruct(unsigned char* screen);
void inventoryUnpack__Fiib(int, int, bool);
int netPlayerIsLocal__Fi(int index);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
Double litodp(int value);
Double fptodp(float value);
Double dpadd(Double left, Double right);
Double dpmul(Double left, Double right);
Double sin(Double value);
int dptoli(Double value);
}

static inline int& word(unsigned char* base, int off) { return *reinterpret_cast<int*>(base + off); }
static inline float& real(unsigned char* base, int off) { return *reinterpret_cast<float*>(base + off); }

extern "C" void mainMenu__Fii(int n, int p) {
    unsigned char* const S = shop + n * 116;
    shopDrawTaskBar__FP12screenStruct(S);
    updateCursor__FiiiiiP12screenStructi(p, 0, 2, 0, 39, S, 1);
    word(S, 84) = word(S, 24);
    if (word(S, 24) == 0) {
        textureDrawColor__FP8_textureiiiUi(
            lumpFindResource__FPcT0(const_cast<char*>("shop.lmp"), const_cast<char*>("buttontop.tex")),
            word(S, 0) + 66, 210, 0, 0x80808080u);
    } else {
        textureDrawColor__FP8_textureiiiUi(
            lumpFindResource__FPcT0(const_cast<char*>("shop.lmp"), const_cast<char*>("buttonbottom.tex")),
            word(S, 0) + 66, 248, 0, 0x80808080u);
    }
    const int center = word(S, 0) + word(S, 8) / 2;
    fontStart__FP4FontiUii(defaultFont, 0, 0x80323c3cu, -1);
    if (word(S, 24) == 0) {
        fontColor__FUiii(0x8049616au, -1, 0);
        fontPrintCenteredu__FiiPUsi(center, 203, textGetString__FPc(const_cast<char*>("Buy")), 1000000);
        if ((*reinterpret_cast<int*>(netLocalPad + 88 * p + 12) & padAcceptButton) != 0 && real(S, 60) == 0.0f) {
            resetMenu__FP12screenStruct(S);
            word(S, 76) = 0;
        }
    } else {
        fontColor__FUiii(0x80323c3cu, -1, 0);
        fontPrintCenteredu__FiiPUsi(center, 203, textGetString__FPc(const_cast<char*>("Buy")), 1000000);
    }
    const int selected = word(S, 24);
    if (selected == 1) {
        fontColor__FUiii(0x8049616au, -1, 0);
        fontPrintCenteredu__FiiPUsi(center, 243, textGetString__FPc(const_cast<char*>("Sell")), 1000000);
        if ((*reinterpret_cast<int*>(netLocalPad + 88 * p + 12) & padAcceptButton) != 0 && real(S, 60) == 0.0f) {
            inventoryUnpack__Fiib(p, n, false);
            resetMenu__FP12screenStruct(S);
            word(S, 76) = selected;
        }
    } else {
        fontColor__FUiii(0x80323c3cu, -1, 0);
        fontPrintCenteredu__FiiPUsi(center, 243, textGetString__FPc(const_cast<char*>("Sell")), 1000000);
    }
    if (shopDisableMouse[n] == 0 && shopMouseDecay[n] == 0 &&
        (*reinterpret_cast<int*>(netLocalPad + 88 * p + 12) & padBackButton) != 0) {
        if (netPlayerIsLocal__Fi(p) != 0) {
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
                lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("menu_shop_back.anm")),
                0, 1.0f);
        }
        word(S, 76) = 2;
    }
    fontEnd__Fv();
    if (word(S, 76) != 3) return;
    textureDrawColor__FP8_textureiiiUi(
        lumpFindResource__FPcT0(const_cast<char*>("shop.lmp"), const_cast<char*>("buysell.tex")),
        word(S, 0) + 60, 174, 0, 0x80808080u);
    int shift = 0;
    int base = 0;
    if (language == 3) {
        shift = 6;
        base = -5;
    } else if (language == 4) {
        shift = 5;
        base = -3;
    } else if (language == 1) {
        shift = 9;
        base = -3;
    }
    void* const right = rcursor;
    const float step = __builtin_bit_cast(float, 0x3dccccccu);
    const int width = *reinterpret_cast<short*>(right);
    const Double amplitude = litodp(base + 10);
    const Double phase = dpmul(litodp(p), 0x3ff8000000000000ull);
    int offset = dptoli(dpmul(amplitude, sin(dpadd(phase, fptodp(static_cast<float>(drawFrameNm) * step)))));
    textureDrawColor__FP8_textureiiiUi(right, center - shift - width - offset - 38, word(S, 36) + 206, 1, 0x80808080u);
    void* const left = lcursor;
    offset = dptoli(dpmul(amplitude, sin(dpadd(phase, fptodp(static_cast<float>(drawFrameNm) * step)))));
    textureDrawColor__FP8_textureiiiUi(left, center + offset + shift + 44, word(S, 36) + 206, 1, 0x80808080u);
}
