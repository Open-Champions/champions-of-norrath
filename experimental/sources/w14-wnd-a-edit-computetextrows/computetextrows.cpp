// Observed: ComputeTextRows__9VIWndEdit at 0x010acbd0 is 564 bytes. With this and mUI the word _5VIWnd$mUI: the text is
// the 16-bit string whose pointer is the word at +424, the signed word at +428 is n, the row table is the array of
// 8-byte records whose pointer is the word at +432, and the count of used rows is the word at +436.
//  1. For each of the first n records (nothing when n <= 0): word at +0 = 0 and the 16-bit values at +4 and +6 = 0.
//  2. The row count is set to 0. Starting with row start = text, index 0 and "last space" 0, while text[index] != 0:
//     - a space at the index sets last space = index;
//     - a newline (10) appends the record {start, length = (index - start) + 1, flag 1}, advances index by one,
//       starts the next row at the new index, and clears last space;
//     - any other character: the units from the row start up to (not including) the index are copied by memcpy into
//       a zero-terminated stack buffer and measured with GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(
//       mUI + 56, the word mUI + 72, the word mUI + 80, buffer); when the width is below 389 the index advances.
//       Otherwise (389 or more) a record {start, length, flag 1} is appended. With a nonzero last space the length is
//       (last space - start) + 1 and the index becomes last space + 1; with last space zero the length is
//       (index - start) and the index is unchanged. The next row starts at the new index and last space is cleared.
//  3. When the row start is not at the final index, one last record {start, length = index - start} is appended
//     (its flag stays 0).
//  Records keep the row start as a 32-bit address word. The result is not used.
// Inferred: this wraps the edit text at a pixel width of 389 into rows for drawing and caret placement (see
// GetCaretPos__9VIWndEditP8VIVect2I); the flag marks a row that ended at a line break or a space wrap.
// Unknown: the field names, whether 389 is a width in pixels, a space at index 0 (indistinguishable from no space),
// and the result type (void here).
extern "C" {
extern unsigned _5VIWnd$mUI;
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(void* font, void* raster, void* dictionary,
                                                          const unsigned short* text);
void* memcpy(void* destination, const void* source, unsigned size);

struct EditRow {
    unsigned start;
    short length;
    short flag;
};

static inline unsigned char* ptr(unsigned word) {
    return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(word));
}

void ComputeTextRows__9VIWndEdit(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* ui = ptr(_5VIWnd$mUI);
    const int capacity = *reinterpret_cast<int*>(bytes + 428);
    EditRow* rows = reinterpret_cast<EditRow*>(ptr(*reinterpret_cast<unsigned*>(bytes + 432)));
    for (int i = 0; i < capacity; ++i) {
        rows[i].start = 0;
        rows[i].length = 0;
        rows[i].flag = 0;
    }
    int* row_count = reinterpret_cast<int*>(bytes + 436);
    const unsigned short* start = reinterpret_cast<const unsigned short*>(ptr(*reinterpret_cast<unsigned*>(bytes + 424)));
    *row_count = 0;
    int index = 0;
    int last_space = 0;
    const unsigned short* text = start;
    if (text[0] != 0) {
        do {
            text = reinterpret_cast<const unsigned short*>(ptr(*reinterpret_cast<unsigned*>(bytes + 424)));
            const unsigned short c = text[index];
            if (c == 32) last_space = index;
            if (c == 10) {
                EditRow* row = reinterpret_cast<EditRow*>(ptr(*reinterpret_cast<unsigned*>(bytes + 432))) + *row_count;
                ++index;
                last_space = 0;
                row->start = static_cast<unsigned>(reinterpret_cast<unsigned long>(start));
                row->flag = 1;
                const int length = static_cast<int>((text + index - 1) - start) + 1;
                ++*row_count;
                row->length = static_cast<short>(length);
                start = text + index;
            } else {
                unsigned short buffer[256];
                const int units = static_cast<int>((text + index) - start);
                memcpy(buffer, start, units * 2);
                buffer[units] = 0;
                const int width = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(
                    ui + 56, ptr(*reinterpret_cast<unsigned*>(ui + 72)), ptr(*reinterpret_cast<unsigned*>(ui + 80)), buffer);
                if (width < 389) {
                    ++index;
                } else {
                    EditRow* row = reinterpret_cast<EditRow*>(ptr(*reinterpret_cast<unsigned*>(bytes + 432))) + *row_count;
                    row->start = static_cast<unsigned>(reinterpret_cast<unsigned long>(start));
                    text = reinterpret_cast<const unsigned short*>(ptr(*reinterpret_cast<unsigned*>(bytes + 424)));
                    int length;
                    if (last_space != 0) {
                        row->flag = 1;
                        index = last_space + 1;
                        length = static_cast<int>((text + last_space) - start) + 1;
                        start = text + index;
                    } else {
                        row->flag = 1;
                        length = static_cast<int>((text + index) - start);
                        start = text + index;
                    }
                    row->length = static_cast<short>(length);
                    ++*row_count;
                    last_space = 0;
                }
            }
            text = reinterpret_cast<const unsigned short*>(ptr(*reinterpret_cast<unsigned*>(bytes + 424)));
        } while (text[index] != 0);
    }
    const int rest = static_cast<int>((text + index) - start);
    if (rest != 0) {
        EditRow* row = reinterpret_cast<EditRow*>(ptr(*reinterpret_cast<unsigned*>(bytes + 432))) + *row_count;
        ++*row_count;
        row->start = static_cast<unsigned>(reinterpret_cast<unsigned long>(start));
        row->length = static_cast<short>((text + index) - start);
    }
}
}
