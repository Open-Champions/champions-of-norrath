// Observed: OnDrawSelf__9VIWndEdit at 0x010aa9f8 is 6588 bytes. It draws the text-entry window and its on-screen
// keyboard. Words are named by their byte offset in this. ui is the word _5VIWnd$mUI (0x01192c70) read once, font
// is ui + 56, raster and dictionary are the words at ui + 72 and ui + 80, and the 16-byte colors black {0, 0, 0, a}
// and white {1, 1, 1, a} use the fade value a written in step 2. Structure arguments of the Blit and Draw calls are
// passed as addresses of caller-made copies (observed in the argument registers), so the declarations below take
// pointers. Order of events:
//  1. now = GetTime__7VITimer(); dt = (float)(now - the 64-bit value at +376) * 0.001f; +376 = now;
//     memset of a 256-byte local block to zero, then the two 64-byte frame parameter blocks are filled (below).
//  2. +360 = min(sTranslucency, +360 + 5 dt); +352 = max(+352 - 5 dt, 0). With +368 equal to -1: +364 = max(+364 -
//     2 dt, 0) and +368 = 1 when +364 reaches 0; otherwise +364 = min(+364 + 3 dt, 1) and +368 = -1 when +364 reaches 1.
//  3. compact = 0, or (gKeyboardAttached nonzero) byte 145 of VIGetGameOptions__Fv() != 0; sCompactKeyboard =
//     compact. origin = the words at +56 and +60; rows = the word at +392. The words at +64 and +68 become 452 and
//     17 * rows + (compact ? 42 + 26 : 203).
//  4. When compact is zero: when the 64-bit values at +328 and +336 differ, +356 grows by 15 dt and, at 1 or more,
//     becomes 0 while the words at +336/+340 take the words at +328/+332. A held key in +384 from 136 to 139
//     decrements the float at +388 by dt; at or below 0 it becomes 0.05 and MoveCursor__9VIWndEditii(this, -1, 0 /
//     1, 0 / 0, -1 / 0, 1) is called for 136 / 137 / 138 / 139.
//  5. Begin2D__4VIUI(ui). A non-empty 16-bit title at +72 gets BlitBar (textures 33 and 34) centered above the
//     window. Then BlitRect (51), BlitFrame (frame block A, 36..42 pairs), BlitFillFrame (96, frame block B).
//  6. Not compact: three or four button rows below the keyboard (icons 193, 194, 71, 69 when the word at +444 is
//     nonzero, 72; texts 905 909 908 907 753 through snprintf16 with "= %S") then the 6 x 11 key grid with 27-pixel
//     cells at (origin.x + 16, origin.y + 17 rows + 28): a cell whose sKeyMap byte (sKeyMap at 0x01192b80) is zero is
//     skipped; others get background 107 (and 204 faded by +364 for code 5), a code of 32 or more is drawn as a
//     centred character twice (black then white, offset by one pixel; upper-case letters become lower-case by the
//     rule of the key handler), codes 1..16 get an icon (textures 154 155 112 -- 156 111 226 226 113 113 169 170 171
//     172 114 171) and the cell at (+344, +348) gets the highlight 108. After the grid the moving cursor icon 89 is
//     placed between the cells at (+336, +340) and (+328, +332) by an ease-out of +356.
//     Compact: icon 194 and the text 909 ("%S"), icon 72 and the text 753, and, when +440 is nonzero, the text
//     "%S+%S = %S" built from 905, 906, 909.
//  7. ComputeTextRows; GetCaretPos; the first visible row +400 is moved to keep the caret row visible; each visible
//     row of the line table (pointer +432, count +436, 8-byte entries of text pointer and 16-bit length) is copied to
//     a local buffer (newline becomes a space; with +396 equal to 1 every other unit becomes '*'), drawn black one
//     pixel down-right and white; the caret rectangle follows the caret column. Every second half second (GetTime -
//     the 64-bit value at +416) / 500 even) FillRectangle draws the caret with white.
//  8. End2D__4VIUI(ui).
// Inferred: window position (+56/+60), size (+64/+68), row count (+392), the fade and slide values and the
// button/icon roles are from usage; the texture, string and tone numbers are plain integers.
// Unknown: names of every field and texture, the meaning of the colour flags, and what the float at sTranslucency
// (0x01192bc4, 1.0 in the image) is set from; the return type (void). Constants 0.001f 0.05f 0.6f 0.8f use the
// truncated bit patterns of the image (0x3a83126e 0x3d4ccccc 0x3f199999 0x3f4ccccc).
extern "C" {
struct Vect2 { int x, y; };
struct Color { float r, g, b, a; };
struct FrameParam { int v[16]; };

unsigned long long GetTime__7VITimer();
unsigned char* VIGetGameOptions__Fv();
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(void* font, void* raster, void* dictionary, const unsigned short* text);
int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, void* raster, void* dictionary);
void BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(
    void* ui, const unsigned short* text, const Color* color, const Vect2* position, int first, int second, int width,
    int mode, float alpha, unsigned flags, int extra);
void BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void* ui, int texture, const Vect2* position,
                                                             const Vect2* size, int mode, float alpha);
void BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void* ui, const Vect2* position, const Vect2* size,
                                                                       const FrameParam* frame, int mode, float alpha,
                                                                       unsigned char flag);
void BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(
    void* ui, const Vect2* position, const Vect2* size, const FrameParam* frame, int texture, int mode, float alpha,
    unsigned char flag);
void BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void* ui, int texture, const Vect2* position, int mode,
                                                           float alpha, unsigned flags);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
int snprintf16__8VIStringPUsiPCce(unsigned short* dest, int size, const char* format, ...);
void DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(void* ui, const unsigned short* text, int x, int y, const Color* color, int flag);
void SetTextColor__6VIFontRC10VIColor32F(void* font, const Color* color);
void DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi(void* font, void* raster, void* dictionary,
                                                                 const char* text, int x, int y, unsigned flags);
void DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(void* font, void* raster, void* dictionary,
                                                        const unsigned short* text, int x, int y);
void FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(void* ui, const Vect2* corners, const Color* color);
void MoveCursor__9VIWndEditii(void* self, int dx, int dy);
void ComputeTextRows__9VIWndEdit(void* self);
void GetCaretPos__9VIWndEditP8VIVect2I(void* self, Vect2* position);
void* memcpy(void* dest, const void* source, unsigned long count);
void* memset(void* dest, int value, unsigned long count);
extern unsigned _5VIWnd$mUI;
extern int gKeyboardAttached;
extern int sCompactKeyboard;
extern float sTranslucency;
extern unsigned char sKeyMap[];

static inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
static inline void* pointer_of(unsigned word) { return reinterpret_cast<void*>(static_cast<unsigned long>(word)); }
static inline unsigned char* ui_bytes() { return static_cast<unsigned char*>(pointer_of(_5VIWnd$mUI)); }
static inline const unsigned short* string_for(int id) {
    const void* table = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(table, id, static_cast<const int*>(table)[12]);
}
static inline void* raster_of() { return pointer_of(*reinterpret_cast<unsigned*>(ui_bytes() + 72)); }
static inline void* dictionary_of() { return pointer_of(*reinterpret_cast<unsigned*>(ui_bytes() + 80)); }

// Lower-case rule shared with the key handler: an upper-case key cap is lowered unless a shift state is active.
static inline bool lowered(const int* words, const unsigned char* bytes) {
    const int caps = words[11];
    if (caps == 0 && *reinterpret_cast<const unsigned long long*>(bytes + 48) == 0) return true;
    const int shifted = words[13];
    if (shifted != 1) return false;
    if (caps == shifted) return true;
    return words[12] == shifted;
}

struct Icon { int texture; int dx; int dy; unsigned flags; };

void OnDrawSelf__9VIWndEdit(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    float* floats = static_cast<float*>(self);
    void* const ui = pointer_of(_5VIWnd$mUI);
    void* const font = ui_bytes() + 56;
    const unsigned long long now = GetTime__7VITimer();
    Color black = {0.0f, 0.0f, 0.0f, floats[90]};
    Color white = {1.0f, 1.0f, 1.0f, floats[90]};
    const float dt = static_cast<float>(static_cast<long long>(now - *reinterpret_cast<unsigned long long*>(bytes + 376))) *
                     constant(0x3a83126eu);
    *reinterpret_cast<unsigned long long*>(bytes + 376) = now;
    int block[64];
    memset(block, 0, 256);
    {
        static const int a[16] = {36, 0, 37, 0, 36, 1, 42, 1, 36, 1, 37, 0, 36, 0, 42, 0};
        static const int b[16] = {101, 0, 102, 0, 103, 0, 104, 1, 106, 0, 102, 2, 105, 0, 104, 0};
        for (int i = 0; i < 16; ++i) {
            block[i] = a[i];
            block[48 + i] = b[i];
        }
    }
    const FrameParam* const frame_a = reinterpret_cast<const FrameParam*>(block);
    const FrameParam* const frame_b = reinterpret_cast<const FrameParam*>(block + 48);

    const float step = dt * 5.0f;
    float fade = floats[90] + step;
    float slide = floats[88] - step;
    if (!(slide > 0.0f)) slide = 0.0f;
    floats[88] = slide;
    if (sTranslucency < fade) fade = sTranslucency;
    floats[90] = fade;
    black.a = fade;
    white.a = fade;
    if (words[92] == -1) {
        float f = floats[91] - (dt + dt);
        if (!(f > 0.0f)) f = 0.0f;
        floats[91] = f;
        if (f == 0.0f) words[92] = 1;
    } else {
        float f = floats[91] + dt * 3.0f;
        if (!(f < 1.0f)) f = 1.0f;
        floats[91] = f;
        if (f == 1.0f) words[92] = -1;
    }

    int compact = 0;
    if (gKeyboardAttached != 0) compact = VIGetGameOptions__Fv()[145] != 0;
    Vect2 origin = {words[14], words[15]};
    sCompactKeyboard = compact;
    Vect2 size = {452, 17 * words[98] + (compact ? 42 : 203)};
    if (sCompactKeyboard != 0) size.y += 26;
    words[16] = size.x;
    words[17] = size.y;

    if (sCompactKeyboard == 0) {
        if (*reinterpret_cast<long long*>(bytes + 328) != *reinterpret_cast<long long*>(bytes + 336)) {
            const float v = floats[89] + dt * 15.0f;
            floats[89] = v;
            if (1.0f <= v) {
                words[89] = 0;
                words[84] = words[82];
                words[85] = words[83];
            }
        }
        const int key = words[96];
        if (key < 140 && key >= 136) {
            const float v = floats[97] - dt;
            floats[97] = v;
            if (v <= 0.0f) {
                floats[97] = constant(0x3d4ccccc);
                if (key == 138) MoveCursor__9VIWndEditii(self, 0, -1);
                else if (key == 139) MoveCursor__9VIWndEditii(self, 0, 1);
                else if (key == 136) MoveCursor__9VIWndEditii(self, -1, 0);
                else if (key == 137) MoveCursor__9VIWndEditii(self, 1, 0);
            }
        }
    }

    Begin2D__4VIUI(ui);
    if (*reinterpret_cast<unsigned short*>(bytes + 72) != 0) {
        const unsigned short* title = reinterpret_cast<const unsigned short*>(bytes + 72);
        const int width = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(font, raster_of(), dictionary_of(), title) + 50;
        const Vect2 where = {origin.x + (size.x >> 1) - (width >> 1), origin.y - 28};
        BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(ui, title, &white, &where, 33, 34, width,
                                                                                     3, floats[90], 0, 1);
    }
    BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(ui, 51, &origin, &size, 3, floats[90]);
    BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(ui, &origin, &size, frame_a, 3, floats[90], 1);
    {
        const Vect2 fill_position = {origin.x + 16, origin.y + 16};
        const Vect2 fill_size = {420, 17 * words[98] + 8};
        BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(ui, &fill_position, &fill_size,
                                                                                        frame_b, 96, 3, floats[90], 1);
    }

    unsigned short line[128];
    if (sCompactKeyboard == 0) {
        const float faded = floats[90] * constant(0x3f400000);
        Vect2 cursor = {origin.x + 325, origin.y + 17 * words[98] + 28};
        {
            const Vect2 icon = {origin.x + 320, origin.y + 17 * words[98] + 32};
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 193, &icon, 3, faded, 0);
        }
        snprintf16__8VIStringPUsiPCce(line, 128, "= %S", string_for(905));
        DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, line, cursor.x + 30, cursor.y, &white, 1);
        {
            const Vect2 icon = {cursor.x - 5, cursor.y + 31};
            cursor.y += 27;
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 194, &icon, 3, floats[90] * constant(0x3f400000), 0);
        }
        snprintf16__8VIStringPUsiPCce(line, 128, "= %S", string_for(909));
        DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, line, cursor.x + 30, cursor.y, &white, 1);
        {
            const Vect2 icon = {cursor.x + 3, cursor.y + 32};
            cursor.y += 27;
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 71, &icon, 3, floats[90] * constant(0x3f400000), 0);
        }
        snprintf16__8VIStringPUsiPCce(line, 128, "= %S", string_for(908));
        DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, line, cursor.x + 30, cursor.y, &white, 1);
        if (words[111] != 0) {
            const Vect2 icon = {cursor.x + 3, cursor.y + 32};
            cursor.y += 27;
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 69, &icon, 3, floats[90] * constant(0x3f400000), 0);
            snprintf16__8VIStringPUsiPCce(line, 128, "= %S", string_for(907));
            DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, line, cursor.x + 30, cursor.y, &white, 1);
        }
        {
            const Vect2 icon = {cursor.x + 3, cursor.y + 32};
            cursor.y += 27;
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 72, &icon, 3, floats[90] * constant(0x3f400000), 0);
        }
        snprintf16__8VIStringPUsiPCce(line, 128, "= %S", string_for(753));
        DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, line, cursor.x + 30, cursor.y, &white, 1);

        const Vect2 grid = {origin.x + 16, words[15] + (17 * words[98] + 28)};
        static const Icon icons[16] = {{154, 4, 8, 0}, {155, 4, 8, 0}, {112, 7, 8, 0}, {0, 0, 0, 0}, {156, 4, 8, 0},
                                       {111, 7, 7, 0}, {226, 7, 7, 0}, {226, 7, 7, 2}, {113, 7, 7, 0}, {113, 7, 7, 1},
                                       {169, 9, 10, 0}, {170, 7, 8, 0}, {171, 10, 13, 0}, {172, 7, 7, 0}, {114, 8, 16, 0},
                                       {171, 10, 7, 0}};
        for (int row = 0; row < 6; ++row) {
            for (int col = 0; col < 11; ++col) {
                const Vect2 cell = {grid.x + 27 * col, grid.y + 27 * row};
                const signed char code = static_cast<signed char>(sKeyMap[11 * row + col]);
                if (code == 0) continue;
                BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 107, &cell, 3, floats[90] * constant(0x3f400000), 0);
                if (static_cast<signed char>(sKeyMap[11 * row + col]) == 5)
                    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 204, &cell, 3,
                                                                           (floats[91] * floats[90]) * constant(0x3f400000), 0);
                const unsigned char raw = sKeyMap[11 * row + col];
                if (code >= 32) {
                    char text[2] = {static_cast<char>(raw), 0};
                    if (static_cast<unsigned>(raw - 65) < 26u && lowered(words, bytes)) text[0] = static_cast<char>(raw + 32);
                    SetTextColor__6VIFontRC10VIColor32F(font, &black);
                    DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi(font, raster_of(), dictionary_of(), text,
                                                                                cell.x + 14, cell.y + 12, 0);
                    SetTextColor__6VIFontRC10VIColor32F(font, &white);
                    DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi(font, raster_of(), dictionary_of(), text,
                                                                                cell.x + 13, cell.y + 11, 0);
                } else if (static_cast<unsigned>(code - 1) < 16u && code != 4) {
                    const Icon& icon = icons[code - 1];
                    const Vect2 where = {cell.x + icon.dx, cell.y + icon.dy};
                    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, icon.texture, &where, 3,
                                                                           floats[90] * constant(0x3f4ccccc), icon.flags);
                }
                if (words[86] == col && words[87] == row)
                    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(
                        ui, 108, &cell, 3, (floats[90] * floats[88]) * constant(0x3f199999), 0);
            }
        }
        const Vect2 old_cell = {grid.x + 27 * words[84], grid.y + 27 * words[85]};
        const Vect2 new_cell = {grid.x + 27 * words[82], grid.y + 27 * words[83]};
        const float inverse = 1.0f - floats[89];
        const float ease = 1.0f - inverse * inverse;
        const float move_x = static_cast<float>(new_cell.x - old_cell.x) * ease;
        const float move_y = static_cast<float>(new_cell.y - old_cell.y) * ease;
        const Vect2 mark = {old_cell.x + static_cast<int>(move_x) - 4, old_cell.y + static_cast<int>(move_y) + 5};
        BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 89, &mark, 3, floats[90], 0);
    } else {
        Vect2 cursor = {words[14] + 16, words[15] + (17 * words[98] + 28)};
        {
            const Vect2 icon = {cursor.x + 3, cursor.y + 4};
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 194, &icon, 3, floats[90] * constant(0x3f400000), 0);
        }
        snprintf16__8VIStringPUsiPCce(line, 128, "%S", string_for(909));
        DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, line, cursor.x + 39, cursor.y, &white, 1);
        cursor = Vect2{words[14] + 16, words[15] + (17 * words[98] + 28)};
        {
            const Vect2 icon = {cursor.x + 108, cursor.y + 5};
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 72, &icon, 3, floats[90] * constant(0x3f400000), 0);
        }
        snprintf16__8VIStringPUsiPCce(line, 128, "%S", string_for(753));
        DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, line, cursor.x + 105 + 23, cursor.y, &white, 1);
        if (words[110] != 0) {
            cursor = Vect2{words[14] + 16, words[15] + (17 * words[98] + 28)};
            const unsigned short* first = string_for(905);
            const unsigned short* second = string_for(906);
            const unsigned short* third = string_for(909);
            snprintf16__8VIStringPUsiPCce(line, 128, "%S+%S = %S", first, second, third);
            DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, line, cursor.x + 260, cursor.y, &white, 1);
        }
    }

    ComputeTextRows__9VIWndEdit(self);
    Vect2 caret = {0, 0};
    GetCaretPos__9VIWndEditP8VIVect2I(self, &caret);
    int top = words[100];
    if (caret.y < top) {
        words[100] = caret.y;
        top = caret.y;
    } else if (!(caret.y < top + words[98])) {
        top = caret.y - words[98] + 1;
        words[100] = top;
    }
    Vect2 mark_corners[2] = {{words[14] + 22, words[15] + 21}, {0, 0}};
    int line_y = words[15] + 16;
    unsigned short text_line[256];
    for (int row = top; row < top + words[98]; ) {
        if (row < words[109]) {
            const unsigned char* entry = static_cast<const unsigned char*>(pointer_of(static_cast<unsigned>(words[108]))) + row * 8;
            const int length = *reinterpret_cast<const short*>(entry + 4);
            memcpy(text_line, pointer_of(*reinterpret_cast<const unsigned*>(entry)), static_cast<unsigned long>(length) * 2);
            text_line[length] = 0;
            for (int i = 0; i < length; ++i) {
                if (text_line[i] == 10) text_line[i] = 32;
                else if (words[99] == 1) text_line[i] = 42;
            }
            SetTextColor__6VIFontRC10VIColor32F(font, &black);
            DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(font, raster_of(), dictionary_of(), text_line, words[14] + 23,
                                                               line_y + 1);
            SetTextColor__6VIFontRC10VIColor32F(font, &white);
            DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(font, raster_of(), dictionary_of(), text_line, words[14] + 22,
                                                               line_y);
        }
        if (caret.y == row) {
            text_line[caret.x] = 0;
            const int width = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(font, raster_of(), dictionary_of(), text_line);
            mark_corners[0].x = words[14] + width + 22;
            mark_corners[0].y = line_y + 5;
        }
        const int height = GetHeight__6VIFontP8VIRasterP12VIDictionary(font, raster_of(), dictionary_of());
        ++row;
        line_y = line_y + height - 4;
        if (!(row < words[100] + words[98])) break;
    }
    const long long beat = static_cast<long long>(GetTime__7VITimer() - *reinterpret_cast<unsigned long long*>(bytes + 416)) / 500;
    if ((beat & 1) == 0) {
        mark_corners[1].x = mark_corners[0].x + 2;
        mark_corners[1].y = mark_corners[0].y + GetHeight__6VIFontP8VIRasterP12VIDictionary(font, raster_of(), dictionary_of()) - 5;
        FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(ui, mark_corners, &white);
    }
    End2D__4VIUI(ui);
}
}
