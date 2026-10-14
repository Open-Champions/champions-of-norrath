// Observed: TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi at 0x0103c660, 3008 bytes. Arguments: the window (word 0 the UI
// object, ints at +12 x, +16 y, +20 w, +24 h, +28 inner offset), a 16-bit text, ints x, row, width and rows, a colour
// reference and a flags word. CountLines__10VIUIWindowPCUsi calls it with (0, 0, n, 256, the colour at +304, 0xffffffff).
// The result is the number of lines laid out and counted (drawn lines, not the skipped ones). In order:
//  - h1 and h2 are two calls of GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, word at ui + 72, word at ui + 80);
//    X0 = w12 - (w20 >> 1), Y0 = w16 - (w24 >> 1) (arithmetic shifts); left = X0 + x. (The original also computes a box
//    height, (h1 - 4) * rows, and several position pairs into locals that are never read; they are not reproduced.)
//  - When flags is not 0xffffffff and (flags & 4): DrawFrame__10VIUIWindowiiiiUi(window, x, row, width, rows, flags).
//  - skip = flags >> 24 (0 when flags is 0xffffffff). Lines are cut from the text by this scan, with index i, line start
//    `start`, the last space index lastSpace and the last index lastGrow at which the measured width changed (both 0 at
//    each new line; 0 doubles as "none"), and the measured width W (kept across lines):
//    * end of text: the remaining text (if any) is one last line, with no ellipsis and no further scanning;
//    * a newline (10) ends the line [start, i); the line is cut by copying it to a 256-unit buffer with
//      strncpy16__8VIStringPUsUiPCUsi(buffer, 512, text + start, n), or, when flags is not 0xffffffff and (flags & 0x80),
//      with CopyAsteriks__10VIUIWindowPUsii(window, buffer, 512, n), then terminating it; the next line starts after the
//      newline;
//    * any other character: the text [start, i) is copied to the buffer the same way and W' = TextWidth__10VIUIWindowPCUs
//      (window, buffer); when W' < width - 8 the character is accepted: lastGrow = i when W' differs from the previous W,
//      lastSpace = i when the character is a space (32), and i advances; otherwise the line is cut at lastSpace (line
//      [start, lastSpace], next line starts after the space) or, when lastSpace is 0, at lastGrow (line [start, lastGrow),
//      next line starts at lastGrow).
//  - Each line cut inside the scan: when flags is not 0xffffffff and (flags & 2) and lines + 1 == rows and the text after the
//    cut is not empty, AppendEllipsis__10VIUIWindowPUsi(window, buffer, width) is called on it. The line is then skipped when
//    the number of lines processed so far is below skip; otherwise, when flags is not 0xffffffff, it is drawn (see below) and
//    in either case it counts as a line. The processed count rises for every cut line. The scan stops at once when the counted
//    lines reach rows, otherwise lastSpace and lastGrow are reset and it continues.
//  - Drawing a line: with flags & 8 centred: x = left + (width >> 1) - (TextWidth(buffer) >> 1); with flags & 0x10 right
//    aligned: x = left + width - TextWidth(buffer); otherwise x = left. y = Y0 + inner + (hh - 4) * (row + lines) + 5 with
//    hh from a fresh GetHeight call and lines the lines counted so far. DrawTextInternal__10VIUIWindowPCUsiiRC10VIColor32FUi
//    (window, buffer, x, y, colour, flags) is called.
// Inferred: a word-wrapping text box with optional frame (flag 4), ellipsis on the last row (flag 2), centring (8), right
// alignment (0x10), asterisk masking of the text (0x80) and a scroll offset in the top byte; all-ones flags make it a
// measuring pass only.
// Unknown: the names of the flag bits, the width limit's "- 8" margin, and what happens for a text wider than the buffer.
extern "C" {
struct VIColor32F {
    float v[4];
};
int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, void* raster, void* dictionary);
void DrawFrame__10VIUIWindowiiiiUi(void* self, int a, int b, int c, int d, unsigned flags);
void CopyAsteriks__10VIUIWindowPUsii(void* self, unsigned short* destination, int size, int length);
void strncpy16__8VIStringPUsUiPCUsi(unsigned short* destination, unsigned size, const unsigned short* source, int length);
int TextWidth__10VIUIWindowPCUs(void* self, const unsigned short* text);
int strlen16__8VIStringPCUs(const unsigned short* text);
void AppendEllipsis__10VIUIWindowPUsi(void* self, unsigned short* text, int width);
void DrawTextInternal__10VIUIWindowPCUsiiRC10VIColor32FUi(void* self, const unsigned short* text, int x, int y,
                                                          const VIColor32F& color, unsigned flags);

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

static inline int font_height(const int* words) {
    unsigned char* ui = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(words[0]));
    return GetHeight__6VIFontP8VIRasterP12VIDictionary(
        ui + 56, NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(ui + 72)),
        NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(ui + 80)));
}

static inline void copy_line(void* self, unsigned short* buffer, const unsigned short* text, int length, unsigned flags) {
    if (flags != 0xffffffffu && (flags & 0x80) != 0) CopyAsteriks__10VIUIWindowPUsii(self, buffer, 512, length);
    else strncpy16__8VIStringPUsUiPCUsi(buffer, 512, text, length);
    buffer[length] = 0;
}

static inline void draw_line(void* self, unsigned short* buffer, int left, int width, int row, int lines,
                             const VIColor32F& color, unsigned flags) {
    const int* words = static_cast<const int*>(self);
    int x = left;
    if ((flags & 8) != 0) {
        const int text_width = TextWidth__10VIUIWindowPCUs(self, buffer);
        x = left + (width >> 1) - (text_width >> 1);
    } else if ((flags & 0x10) != 0) {
        const int text_width = TextWidth__10VIUIWindowPCUs(self, buffer);
        x = left + width - text_width;
    }
    const int height = font_height(words);
    const int y = words[4] - (words[6] >> 1) + words[7] + (height - 4) * (row + lines) + 5;
    DrawTextInternal__10VIUIWindowPCUsiiRC10VIColor32FUi(self, buffer, x, y, color, flags);
}

int TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* self, const unsigned short* text, int x, int row, int width, int rows,
                                                  const VIColor32F& color, unsigned flags) {
    const int* words = static_cast<const int*>(self);
    font_height(words);
    const int left = words[3] - (words[5] >> 1) + x;
    font_height(words);
    if (flags != 0xffffffffu && (flags & 4) != 0) DrawFrame__10VIUIWindowiiiiUi(self, x, row, width, rows, flags);
    const int skip = flags == 0xffffffffu ? 0 : static_cast<int>(flags >> 24);
    unsigned short buffer[256];
    int lines = 0;
    int processed = 0;
    int measured = 0;
    int index = 0;
    int start = 0;
    int last_space = 0;
    int last_grow = 0;
    while (text[index] != 0) {
        if (text[index] == 10) {
            copy_line(self, buffer, text + start, index - start, flags);
            index += 1;
            start = index;
        } else {
            copy_line(self, buffer, text + start, index - start, flags);
            const int previous = measured;
            measured = TextWidth__10VIUIWindowPCUs(self, buffer);
            if (measured < width - 8) {
                if (previous != measured) last_grow = index;
                if (text[index] == 32) last_space = index;
                index += 1;
                continue;
            }
            if (last_space != 0) {
                copy_line(self, buffer, text + start, last_space - start + 1, flags);
                index = last_space + 1;
            } else {
                copy_line(self, buffer, text + start, last_grow - start, flags);
                index = last_grow;
            }
            start = index;
        }
        if (flags != 0xffffffffu && (flags & 2) != 0 && lines + 1 == rows && strlen16__8VIStringPCUs(text + start) != 0)
            AppendEllipsis__10VIUIWindowPUsi(self, buffer, width);
        if (processed >= skip) {
            if (flags != 0xffffffffu) draw_line(self, buffer, left, width, row, lines, color, flags);
            lines += 1;
        }
        processed += 1;
        if (!(lines < rows)) return lines;
        last_space = 0;
        last_grow = 0;
    }
    const int rest = index - start;
    if (rest != 0) {
        copy_line(self, buffer, text + start, rest, flags);
        if (processed >= skip) {
            if (flags != 0xffffffffu) draw_line(self, buffer, left, width, row, lines, color, flags);
            lines += 1;
        }
    }
    return lines;
}
}
