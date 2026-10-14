// Observed: Draw__10VIUIWindow at 0x0103bf30, 1836 bytes. One argument, the window; the result is a 0/1 flag.
// Window fields used: word 0 the UI object (ui), +12 x, +16 y, +20 width, +24 height (ints), +28 unused here,
// the floats at +40 (p) and +44 (rate), the bytes from +48 (a title string, first byte tested as signed char) and
// the int at +360 (style).
//  1. memset(local, 0, 256) is called on a local array of four 16-int blocks, then ids are stored in it. Block 0
//     holds the even words 36, 37, 36, 42, 36, 37, 36, 42 with the odd words 0, 0, 1, 1, 1, 0, 0, 0; block 1 the
//     even words 44, 45, 46, 52, 50, 49, 48, 47 (odd words 0); block 3 the even words 101, 102, 103, 104, 106, 102,
//     105, 104 with the odd words 0, 0, 0, 1, 0, 2, 0, 0. Block 2 gets values (38, 43, 41, 43 and a 1) that no call
//     reads; it is not reproduced beyond the memset.
//  2. p moves toward 1 or 0: when rate > 0 it becomes min(p + 0.15 (bits 0x3e199999), 1.0), otherwise
//     max(p - 0.2 (bits 0x3e4ccccc), 0.0); the result is stored back at +40. alpha = 1 - (1 - p) * (1 - p).
//  3. With x0 = x - (width >> 1) and y0 = y - (height >> 1) (arithmetic shifts), by style:
//     0: BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(ui, (x0+10, y0+10),
//        (width-19, height-20), block 1, texture 35, blend 3, alpha, 1), then BlitFrame__4VIUIG8VIVect2IT1G14
//        VIUIFrameParam16VIAlphaBlendModefUc(ui, (x0, y0), (width, height), block 0, 3, alpha, 1).
//     1: BlitFillFrame(ui, (x0+10, y0+10), (width-20, height-20), block 3, texture 61, 3, alpha * 0.75, 1).
//     2: BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(ui, 51, (x0+10, y0+10), (width-19, height-20), 3,
//        alpha), then the same BlitFrame call as style 0.
//     any other style draws nothing here.
//  4. When the first title byte is nonzero: w = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(ui + 56, word at
//     ui + 72, word at ui + 80, title) + 50, and BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlpha
//     BlendModefUii(ui, title, colour (1, 1, 1, alpha), (x - (w >> 1), y0 + 10 - 21), textures 33 and 34, w,
//     blend 3, alpha, 3, 1) is called.
//  5. The result is 1 when p == 1.0 or p == 0.0, otherwise 0.
// The original also stores many unused pairs in its frame (a copy of the window's rectangle and the like); they
// are not reproduced.
// Inferred: this is the per-frame draw of a pop-up window: it fades in at 0.15 per call or out at 0.2 per
// call, draws one of three background styles inside a frame and an optional title plate, and reports that the
// fade has settled (fully open or fully closed).
// Unknown: the names of the textures and styles, what the frame blocks' odd words mean (taken to be flip flags),
// and the result type (an int 0/1 is returned).
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};
struct VIUIFrameParam {
    int v[16];
};
struct VIColor32F {
    float v[4];
};

void* memset(void* destination, int value, unsigned size);
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void* font, unsigned raster, unsigned dictionary,
                                                         const char* text);
int BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(
    void* ui, VIVect2I at, VIVect2I size, VIUIFrameParam parameter, int texture, int blend, float alpha,
    unsigned char flag);
int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void* ui, VIVect2I at, VIVect2I size,
                                                                        VIUIFrameParam parameter, int blend,
                                                                        float alpha, unsigned char flag);
int BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void* ui, int texture, VIVect2I at, VIVect2I size,
                                                             int blend, float alpha);
int BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(void* ui, const char* name,
                                                                                VIColor32F color, VIVect2I at,
                                                                                int texture_a, int texture_b, int width,
                                                                                int blend, float alpha, unsigned flags,
                                                                                int extra);

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

static VIVect2I vect(int x, int y) {
    VIVect2I result;
    result.x = x;
    result.y = y;
    return result;
}

int Draw__10VIUIWindow(void* self) {
    int* words = static_cast<int*>(self);
    float* floats = static_cast<float*>(self);
    char* chars = static_cast<char*>(self);
    int blocks[64];
    memset(blocks, 0, 256);
    blocks[0] = 36;  blocks[2] = 37;  blocks[4] = 36;  blocks[5] = 1;  blocks[6] = 42;  blocks[7] = 1;
    blocks[8] = 36;  blocks[9] = 1;   blocks[10] = 37; blocks[12] = 36; blocks[14] = 42;
    blocks[16] = 44; blocks[18] = 45; blocks[20] = 46; blocks[22] = 52; blocks[24] = 50; blocks[26] = 49;
    blocks[28] = 48; blocks[30] = 47;
    blocks[48] = 101; blocks[50] = 102; blocks[52] = 103; blocks[54] = 104; blocks[55] = 1; blocks[56] = 106;
    blocks[58] = 102; blocks[59] = 2;   blocks[60] = 105; blocks[62] = 104;

    if (0.0f < floats[11]) {
        float next = floats[10] + __builtin_bit_cast(float, 0x3e199999u);
        floats[10] = next < 1.0f ? next : 1.0f;
    } else {
        float next = floats[10] - __builtin_bit_cast(float, 0x3e4ccccc);
        floats[10] = next > 0.0f ? next : 0.0f;
    }
    void* ui = reinterpret_cast<void*>(NORRATH_WORD_TO_POINTER(static_cast<unsigned>(words[0])));
    const int width = words[5];
    const int height = words[6];
    const int x0 = words[3] - (width >> 1);
    const int y0 = words[4] - (height >> 1);
    const float ease = 1.0f - floats[10];
    const float alpha = 1.0f - ease * ease;
    const int style = words[90];
    VIUIFrameParam* frame0 = reinterpret_cast<VIUIFrameParam*>(blocks);
    VIUIFrameParam* frame1 = reinterpret_cast<VIUIFrameParam*>(blocks + 16);
    VIUIFrameParam* frame3 = reinterpret_cast<VIUIFrameParam*>(blocks + 48);
    if (style == 1) {
        BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(
            ui, vect(x0 + 10, y0 + 10), vect(width - 20, height - 20), *frame3, 61, 3,
            alpha * __builtin_bit_cast(float, 0x3f400000u), 1);
    } else if (style == 0) {
        BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(
            ui, vect(x0 + 10, y0 + 10), vect(width - 20 + 1, height - 20), *frame1, 35, 3, alpha, 1);
        BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(ui, vect(x0, y0), vect(width, height),
                                                                          *frame0, 3, alpha, 1);
    } else if (style == 2) {
        BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(ui, 51, vect(x0 + 10, y0 + 10),
                                                                 vect(width - 20 + 1, height - 20), 3, alpha);
        BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(ui, vect(x0, y0), vect(width, height),
                                                                          *frame0, 3, alpha, 1);
    }
    if (chars[48]) {
        char* title = chars + 48;
        const int title_width =
            GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(
                static_cast<char*>(ui) + 56, *reinterpret_cast<unsigned*>(static_cast<char*>(ui) + 72),
                *reinterpret_cast<unsigned*>(static_cast<char*>(ui) + 80), title) +
            50;
        VIColor32F white;
        white.v[0] = 1.0f;
        white.v[1] = 1.0f;
        white.v[2] = 1.0f;
        white.v[3] = alpha;
        BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(
            ui, title, white, vect(words[3] - (title_width >> 1), y0 + 10 - 21), 33, 34, title_width, 3, alpha, 3, 1);
    }
    return floats[10] == 1.0f || floats[10] == 0.0f;
}
}
