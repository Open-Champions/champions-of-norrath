// Observed: loadThreadMain__FPv at 0x002cd898 is 744 bytes; the argument is not used and it
// returns nothing. engineDisplayDisable = 4 is stored first, whatever happens next.
// When loadingScreenRequestExit (a byte) is nonzero on entry nothing else is drawn;
// otherwise it repeats the following until that byte is nonzero after a frame:
//   engineFrameStart__FUib(0x80000000, 1); then, with colour 0x80808080 and depth 6:
//   * titleloadTexCopy centred on (320, 194): textureDrawColor at x = 320 - w / 2,
//     y = 194 - h / 2 (w, h from texNormalizedWidth / texNormalizedHeight, each
//     divided by two rounding toward zero);
//   * blackoutTexCopy twice, centred vertically on 224: textureDrawColor at x = 0,
//     then textureDrawSuperColor(tex, 320, y, 6, colour, 321, 0, 642, 447, 1, 0);
//   * for i in 0, 1: a foreground strip. n = drawFrameNm % 1520 (signed), with
//     drawFrameNm read again for each i; x = 2 * i * (short at foregrndTexCopy + 0)
//     - n + 640. It is drawn only when x >= 2 * -(short at foregrndTexCopy + 64 * i)
//     and x < 641, using texture foregrndTexCopy + 64 * (i & 1) through
//     textureDrawFloat(tex, (float)x, (float)(224 - hh / 2), 6, colour, 2.0f, 0.99f),
//     hh being the short at foregrndTexCopy + 2 (always the first texture's);
//   * scrolling strips: q = drawFrameNm * 100 / 140; x starts at -(q % 640) and the
//     index at q / 640; while x < 640: draw scrollmapTexCopy + 64 * (index & 1) with
//     textureDrawFloat at x (hh being that texture's short at +2), then
//     x += 640 and index += 1. A negative remainder makes x start positive.
//   engineFrameEnd__Fb(1).
// After the loop engineSetDisplayDisable__Fi(1) runs.
// Inferred: this is the loading-screen thread body that core_start_loading_screen
// starts; the shorts at texture offsets 0 and 2 are the width and height used by the
// normalized-size calls; drawFrameNm is a frame counter driving the scroll.
// Unknown: the structure of the texture records beyond those two fields, the
// meaning of the numeric constants passed to textureDrawSuperColor, and whether
// the thread blocks anywhere other than the frame calls.
extern "C" {
extern int engineDisplayDisable;
extern unsigned char loadingScreenRequestExit;
extern int drawFrameNm;
extern unsigned char titleloadTexCopy[];
extern unsigned char blackoutTexCopy[];
extern unsigned char foregrndTexCopy[];
extern unsigned char scrollmapTexCopy[];
void engineFrameStart__FUib(unsigned color, int flag);
void engineFrameEnd__Fb(int flag);
void engineSetDisplayDisable__Fi(int value);
int texNormalizedWidth__FP8_texture(void* texture);
int texNormalizedHeight__FP8_texture(void* texture);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* texture, int x, int y, int z, unsigned color, int a, int b, int c, int d, char e, char f);
void textureDrawFloat__FP8_textureffiUiff(void* texture, float x, float y, int z, unsigned color, float sx, float sy);

static inline short shortAt(const unsigned char* texture, int offset) {
    return *reinterpret_cast<const short*>(texture + offset);
}

void loadThreadMain__FPv(void* argument) {
    (void)argument;
    engineDisplayDisable = 4;
    if (loadingScreenRequestExit != 0) {
        engineSetDisplayDisable__Fi(1);
        return;
    }
    do {
        engineFrameStart__FUib(0x80000000u, 1);
        int width = texNormalizedWidth__FP8_texture(titleloadTexCopy);
        int height = texNormalizedHeight__FP8_texture(titleloadTexCopy);
        textureDrawColor__FP8_textureiiiUi(titleloadTexCopy, 320 - width / 2, 194 - height / 2, 6, 0x80808080u);
        height = texNormalizedHeight__FP8_texture(blackoutTexCopy);
        textureDrawColor__FP8_textureiiiUi(blackoutTexCopy, 0, 224 - height / 2, 6, 0x80808080u);
        height = texNormalizedHeight__FP8_texture(blackoutTexCopy);
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(blackoutTexCopy, 320, 224 - height / 2, 6, 0x80808080u, 321, 0, 642, 447, 1, 0);
        for (int index = 0; index < 2; ++index) {
            int x = index * shortAt(foregrndTexCopy, 0) * 2 - drawFrameNm % 1520 + 640;
            if (x >= -shortAt(foregrndTexCopy + 64 * index, 0) * 2 && x < 641) {
                int strip = shortAt(foregrndTexCopy, 2);
                textureDrawFloat__FP8_textureffiUiff(foregrndTexCopy + (index & 1) * 64, static_cast<float>(x),
                                                     static_cast<float>(224 - strip / 2), 6, 0x80808080u, 2.0f, 0.99f);
            }
        }
        int steps = drawFrameNm * 100 / 140;
        int x = -(steps % 640);
        int index = steps / 640;
        while (x < 640) {
            unsigned char* texture = scrollmapTexCopy + (index & 1) * 64;
            ++index;
            int strip = shortAt(texture, 2);
            textureDrawFloat__FP8_textureffiUiff(texture, static_cast<float>(x), static_cast<float>(224 - strip / 2),
                                                 6, 0x80808080u, 2.0f, 0.99f);
            x += 640;
        }
        engineFrameEnd__Fb(1);
    } while (loadingScreenRequestExit == 0);
    engineSetDisplayDisable__Fi(1);
}
}
