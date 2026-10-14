// Observed hudShowMsg__FPUsN20iii at 0x001c6788, 708 bytes.
// Arguments: three wide strings (title, body, footer, any may be null), an
// alpha value, a width (zero means 520) and a flag.
// The font is lumpFindResource__FPcT0("frontend.lmp", "gamefontsmall.fnt") when
// the word frontEndRunning (0x00483f30) is nonzero, and the pointer word
// smallFont (0x00484ca4) otherwise. fontStart__FP4FontiUii(font, 6, color, -1)
// starts with color 0x323c3c plus the alpha in the top byte.
// Measuring pass, using fontPrintWrapCenteredu__FiiPUsiiii(320, 212, text, width
// - 50, 100, 100, 17) whose result is a height: the title is measured only when
// the flag is zero (otherwise its height counts as 0); the body is measured when
// present. The middle height is the body height when a body exists and the flag is
// zero, and the flag itself otherwise. The box height is title height plus middle
// height plus 85 with a footer and plus 68 without. The box is centred: top text
// y is 225 - half the box height (integer division by 2), the footer is placed
// at half + 187.
// Drawing pass, each text preceded by fontColor__FUiii(alpha << 24 | rgb, -1, 0):
// the title with 0x1e6464 at (320, top) with the clip pair (-100, 580), the body
// with 0x323c3c at top + title height + 17, or at 212 - half the body height when
// the flag is nonzero, and the footer with 0x3c4646 by
// fontPrintCenteredu__FiiPUsi(320, footer y, footer, 1000000). Then fontEnd__Fv and
// textureDrawBorder__FiiiiiUiPcii(320 - width / 2, 220 - box height / 2, width,
// box height, 6, alpha << 24 | 0x808080, "frontend.lmp", 0, 0).
// The meaning of the wrap arguments and of the numbers 6 and 17 is not
// established here. The callees, frontEndRunning and smallFont are not defined.
extern "C" {
extern int frontEndRunning;
extern void* smallFont;
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, const unsigned short* text, int a, int b, int c, int d);
void fontColor__FUiii(unsigned color, int a, int b);
void fontPrintCenteredu__FiiPUsi(int x, int y, const unsigned short* text, int color);
void fontEnd__Fv();
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned color, const char* name, int b, int c);
}

extern "C" void hudShowMsg__FPUsN20iii(const unsigned short* title, const unsigned short* body,
                                       const unsigned short* footer, int alpha, int width_arg, int flag) {
    const int width = width_arg != 0 ? width_arg : 520;
    void* font = frontEndRunning != 0
        ? lumpFindResource__FPcT0("frontend.lmp", "gamefontsmall.fnt")
        : smallFont;
    const unsigned top = static_cast<unsigned>(alpha) << 24;
    fontStart__FP4FontiUii(font, 6, top | 0x323c3c, -1);
    int title_height = 0;
    if (title != 0 && flag == 0) {
        title_height = fontPrintWrapCenteredu__FiiPUsiiii(320, 212, title, width - 50, 100, 100, 17);
    }
    int body_height = 0;
    if (body != 0) {
        body_height = fontPrintWrapCenteredu__FiiPUsiiii(320, 212, body, width - 50, 100, 100, 17);
    }
    const int middle = (body != 0 && flag == 0) ? body_height : flag;
    const int box_height = title_height + middle + (footer != 0 ? 85 : 68);
    const int half = box_height / 2;
    const int text_top = 225 - half;
    const int footer_y = half + 187;
    if (title != 0) {
        fontColor__FUiii(top | 0x1e6464, -1, 0);
        fontPrintWrapCenteredu__FiiPUsiiii(320, text_top, title, width - 50, -100, 580, 17);
    }
    if (body != 0) {
        fontColor__FUiii(top | 0x323c3c, -1, 0);
        const int y = flag == 0 ? text_top + title_height + 17 : 212 - body_height / 2;
        fontPrintWrapCenteredu__FiiPUsiiii(320, y, body, width - 50, -100, 580, 17);
    }
    if (footer != 0) {
        fontColor__FUiii(top | 0x3c4646, -1, 0);
        fontPrintCenteredu__FiiPUsi(320, footer_y, footer, 1000000);
    }
    fontEnd__Fv();
    textureDrawBorder__FiiiiiUiPcii(320 - width / 2, 220 - box_height / 2, width, box_height, 6,
                                    top | 0x808080, "frontend.lmp", 0, 0);
}
