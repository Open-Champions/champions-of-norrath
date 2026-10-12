// Observed hudRunYesNo__FPUsiiii at 0x001c32f0, 1348 bytes. It draws a
// two-choice box over the game and polls the pads. It returns 0 while
// undecided, 1 or 2 when accepted. Parameters: text (UTF-16), a width
// adjustment, a player number (-1 means any local player), and x and y
// offsets. The player number is clamped to 0 for per-player state.
//
// Observed layout (all offsets from the arguments): the box is width a+400,
// the text is wrapped and centred at x+320, y+150, and the buttons sit 48
// below the text height: left button at x+215, right at x+337, text on them
// at +40 and 32 above the button top, textures 37 above the button text row.
// Per-player state: coreInterfaceBrightness[p] scales every colour,
// yesNoPos[p] (0 or 1) is the highlighted choice and activeDelay[p] is a
// countdown that blocks input, set to 3 after an accept.
// The highlighted choice is drawn brighter: the "yes" string (left) when
// yesNoPos is 1 and the "no" string (right) when it is 0, scaled by 1.4
// (the image holds that value, twice, for the two texts); button textures
// use grey 200 for the highlighted one and 128 otherwise.
// Input: the word at offset 12 of the pad record (88-byte stride, array
// `pad`) for the player, or the OR over local players when the player is -1.
// Mask 0x802000 moves the choice to 0 and mask 0x408000 to 1, each playing
// the "game_butn_browse.anm" sound from "fx.lmp" only when it changes; any
// of padAcceptButton|0x80 accepts: it sets activeDelay to 3, resets
// yesNoPos to 0, calls coreInterfaceFlare(player) and returns old choice+1.
// Inferred: "yes"/"no" are keys of the two button texts as they are passed
// to textGetString. Unknown: the meaning of 7, 580, 21, 1000000 and the
// -100 style arguments beyond being passed through as in the image.
extern "C" {
struct Font;
struct norrath_pad_record {
    int word0;
    int word4;
    int word8;
    unsigned word12;
    unsigned char rest[72];
};
extern int frontEndRunning;
extern Font* mediumFont;
extern float coreInterfaceBrightness[];
extern int yesNoPos[];
extern int activeDelay[];
extern norrath_pad_record pad[];
extern int padAcceptButton;
extern int nmPlayers;

Font* lumpFindResource__FPcT0(const char* lump, const char* name);
unsigned colorScale__FUif(unsigned color, float scale);
void fontStart__FP4FontiUii(Font* font, int mode, unsigned color, int last);
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int width, int a, int b, int c);
void fontColor__FUiii(unsigned color, int a, int b);
unsigned short* textGetString__FPc(const char* key);
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int scale);
void fontEnd__Fv();
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned color, const char* lump,
                                     int b, int c);
int netPlayerIsLocal__Fi(int index);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float gain);
void coreInterfaceFlare__Fi(int index);

static void play_browse_sound() {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "game_butn_browse.anm"), 0,
                                                         1.0f);
}

int hudRunYesNo__FPUsiiii(unsigned short* text, int widthAdjust, int who, int xOffset, int yOffset) {
    int width = widthAdjust + 400;
    int player = who < 0 ? 0 : who;
    int leftX = xOffset + 215;
    int rightX = xOffset + 337;
    int top = yOffset + 150;
    Font* font = frontEndRunning ? reinterpret_cast<Font*>(lumpFindResource__FPcT0("frontend.lmp", "gamefontsmall.fnt"))
                                 : mediumFont;
    float* brightness = &coreInterfaceBrightness[player];
    int* choice = &yesNoPos[player];
    fontStart__FP4FontiUii(font, 7, colorScale__FUif(0x801e6464u, *brightness), -1);
    int height = fontPrintWrapCenteredu__FiiPUsiiii(xOffset + 320, top, text, width, -100, 580, 21);
    int box = height + 48;
    if (*choice == 1) fontColor__FUiii(colorScale__FUif(0x801e6464u, *brightness * 1.4f), -1, 0);
    int rowY = top + box;
    fontPrintCenteredu__FiiPUsi(leftX + 40, rowY - 32, textGetString__FPc("yes"), 1000000);
    if (*choice != 0)
        fontColor__FUiii(colorScale__FUif(0x801e6464u, *brightness), -1, 0);
    else
        fontColor__FUiii(colorScale__FUif(0x801e6464u, *brightness * 1.4f), -1, 0);
    fontPrintCenteredu__FiiPUsi(rightX + 40, rowY - 32, textGetString__FPc("no"), 1000000);
    fontEnd__Fv();

    unsigned shade = *choice == 1 ? 200u : 128u;
    void* texture = lumpFindResource__FPcT0(frontEndRunning ? "frontend.lmp" : "hud.lmp", "button.tex");
    textureDrawColor__FP8_textureiiiUi(texture, leftX, rowY - 37, 7,
                                       colorScale__FUif(shade | (shade << 8) | (shade << 16) | 0x80000000u, *brightness));
    shade = *choice == 0 ? 200u : 128u;
    texture = lumpFindResource__FPcT0(frontEndRunning ? "frontend.lmp" : "hud.lmp", "button.tex");
    textureDrawColor__FP8_textureiiiUi(texture, rightX, rowY - 37, 7,
                                       colorScale__FUif(shade | (shade << 8) | (shade << 16) | 0x80000000u, *brightness));
    textureDrawBorder__FiiiiiUiPcii(xOffset - width / 2 + 312, top - 8, width + 16, box + 16, 7,
                                    colorScale__FUif(0x80808080u, *brightness), frontEndRunning ? "frontend.lmp" : 0, 0,
                                    0);

    if (activeDelay[player] > 0) {
        activeDelay[player] = activeDelay[player] - 1;
        return 0;
    }
    unsigned buttons;
    if (who == -1) {
        buttons = 0;
        for (int i = 0; i < nmPlayers; ++i) {
            if (netPlayerIsLocal__Fi(i)) buttons |= pad[i].word12;
        }
    } else {
        buttons = pad[who].word12;
    }
    if (buttons & 0x802000u) {
        if (*choice == 1) {
            *choice = 0;
            play_browse_sound();
        }
    }
    if (buttons & 0x408000u) {
        if (*choice == 0) {
            *choice = 1;
            play_browse_sound();
        }
    }
    if (buttons & (static_cast<unsigned>(padAcceptButton) | 0x80u)) {
        int previous = *choice;
        activeDelay[player] = 3;
        *choice = 0;
        coreInterfaceFlare__Fi(player);
        return previous + 1;
    }
    return 0;
}
}
