// Observed: MoveCaretUp__9VIWndEdit at 0x010ace08 is 612 bytes. Words are named by byte offset in this; the row
// table is the pointer at +432 with the count at +436, 8-byte entries {16-bit text pointer, signed halfword
// length at +4, halfword at +6 (call it the flag)}; ui is the word _5VIWnd$mUI (0x01192c70).
//  1. The 64-bit value at +416 = GetTime__7VITimer().
//  2. Locate the caret (+404) in the rows: walking from row 0 with start = sum of earlier lengths, the first row
//     whose start + length is not below the caret gives column = caret - start, row = its index; but when that
//     row's flag is nonzero and caret - start equals its length, column = 0 and row = index + 1. When the table
//     is empty or no row qualifies, column and row stay 0.
//  3. Row 0 returns 0 with nothing else changed.
//  4. When the text pointer of the located row is null: +404 = 0 for row 1, else the sum of the lengths of rows
//     0..row-2; returns 1 (the position a row before the empty trailing row starts).
//  5. Otherwise, when the word at +408 is -1 it is set to GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs
//     (ui + 56, word at ui + 72, word at ui + 80, the first column units of the located row followed by a zero).
//     Then, for the previous row, the prefix width of 1, 2, ... length units is measured the same way; at the first
//     prefix s0 + 1 whose width exceeds the word at +408 the caret becomes caret - column - (length - s0); when
//     none does it becomes caret - column - 1. Returns 1.
// Inferred: +408 keeps the pixel column wanted while moving between rows (-1 means "not measured yet"), so the
// move lands on the previous row's character nearest that column; the flag marks a row that ends at a break.
// Unknown: the names of the fields, the meaning of the halfword flag, the return type (only 0 and 1 occur),
// and the capacity of the local 16-bit text buffer (this unit uses 256 units).
extern "C" {
unsigned long long GetTime__7VITimer();
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(void* font, void* raster, void* dictionary, const unsigned short* text);
void* memcpy(void* dest, const void* source, unsigned long count);
extern unsigned _5VIWnd$mUI;

struct EditRow {
    unsigned text;
    short length;
    short flag;
};

static inline void* pointer_of(unsigned word) { return reinterpret_cast<void*>(static_cast<unsigned long>(word)); }

int MoveCaretUp__9VIWndEdit(void* self) {
    int* words = static_cast<int*>(self);
    unsigned char* ui = static_cast<unsigned char*>(pointer_of(_5VIWnd$mUI));
    int column = 0;
    int row = 0;
    *reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(self) + 416) = GetTime__7VITimer();
    const int count = words[109];
    if (count > 0) {
        const int caret = words[101];
        const EditRow* rows = static_cast<const EditRow*>(pointer_of(words[108]));
        int start = 0;
        for (int i = 0; i < count; ++i) {
            const int length = rows[i].length;
            const int end = start + length;
            if (end < caret) {
                start = end;
                continue;
            }
            if (rows[i].flag != 0 && caret - start == length) {
                column = 0;
                row = i + 1;
            } else {
                column = caret - start;
                row = i;
            }
            break;
        }
    }
    if (row == 0) {
        return 0;
    }
    const EditRow* rows = static_cast<const EditRow*>(pointer_of(words[108]));
    if (rows[row].text == 0) {
        words[101] = 0;
        if (row - 1 > 0) {
            int sum = 0;
            for (int k = 0; k < row - 1; ++k) sum += rows[k].length;
            words[101] = sum;
        }
        return 1;
    }
    unsigned short buffer[256];
    void* font = ui + 56;
    void* raster = pointer_of(*reinterpret_cast<unsigned*>(ui + 72));
    void* dictionary = pointer_of(*reinterpret_cast<unsigned*>(ui + 80));
    if (words[102] == -1) {
        memcpy(buffer, pointer_of(rows[row].text), static_cast<unsigned long>(column) * 2);
        buffer[column] = 0;
        words[102] = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(font, raster, dictionary, buffer);
        rows = static_cast<const EditRow*>(pointer_of(words[108]));
    }
    const EditRow* previous = &rows[row - 1];
    for (int s0 = 0; s0 < previous->length; ++s0) {
        const int units = s0 + 1;
        memcpy(buffer, pointer_of(previous->text), static_cast<unsigned long>(units) * 2);
        buffer[units] = 0;
        if (words[102] < GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(font, raster, dictionary, buffer)) {
            words[101] = words[101] - column - (previous->length - s0);
            return 1;
        }
    }
    words[101] = words[101] - column - 1;
    return 1;
}
}
