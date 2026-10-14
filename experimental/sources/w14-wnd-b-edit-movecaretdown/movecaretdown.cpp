// Observed: MoveCaretDown__9VIWndEdit at 0x010ad070 is 652 bytes. Words are named by byte offset in this; the row
// table is the pointer at +432 with the count at +436, 8-byte entries {16-bit text pointer, signed halfword
// length at +4, halfword at +6 (call it the flag)}; ui is the word _5VIWnd$mUI (0x01192c70).
//  1. The 64-bit value at +416 = GetTime__7VITimer().
//  2. The caret (+404) is located in the rows exactly as in MoveCaretUp__9VIWndEdit: column and row, both 0 when
//     the table is empty or no row qualifies; a row whose flag is nonzero and whose length equals caret - start
//     counts as column 0 of the next row.
//  3. With next = the entry after the located row (read even at or past the count): when next's text pointer is
//     null, the result is 0 and nothing changes if the located row's flag is zero, else the caret becomes
//     caret + length - column (the end of the row) and the result is 1.
//  4. Otherwise a null text pointer in the located row returns 0. When the word at +408 is -1 it is set to the
//     GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs width of the first column units of the located row
//     (ui + 56, words at ui + 72 and ui + 80, text with a zero after the prefix). The prefix widths of 1, 2, ...
//     units of next are measured; at the first prefix s4 + 1 whose width exceeds the word at +408 the caret becomes
//     caret + (length - column) + s4; when none does it becomes caret + (length - column) + next length - 1.
//     Returns 1.
// Inferred: the mirror of MoveCaretUp; +408 keeps the wanted pixel column, so the caret lands in the next row on
// the character nearest it, and a last row moves to its end.
// Unknown: field names, the flag's meaning, the return type (only 0 and 1 occur), and the capacity of the local
// 16-bit buffer (this unit uses 256 units).
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

int MoveCaretDown__9VIWndEdit(void* self) {
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
    const EditRow* rows = static_cast<const EditRow*>(pointer_of(words[108]));
    const EditRow* current = &rows[row];
    const EditRow* next = &rows[row + 1];
    if (next->text == 0) {
        if (current->flag == 0) {
            return 0;
        }
        words[101] = words[101] + (current->length - column);
        return 1;
    }
    if (current->text == 0) {
        return 0;
    }
    unsigned short buffer[256];
    void* font = ui + 56;
    void* raster = pointer_of(*reinterpret_cast<unsigned*>(ui + 72));
    void* dictionary = pointer_of(*reinterpret_cast<unsigned*>(ui + 80));
    if (words[102] == -1) {
        memcpy(buffer, pointer_of(current->text), static_cast<unsigned long>(column) * 2);
        buffer[column] = 0;
        words[102] = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(font, raster, dictionary, buffer);
        rows = static_cast<const EditRow*>(pointer_of(words[108]));
        current = &rows[row];
        next = &rows[row + 1];
    }
    for (int s4 = 0; s4 < next->length; ++s4) {
        const int units = s4 + 1;
        memcpy(buffer, pointer_of(next->text), static_cast<unsigned long>(units) * 2);
        buffer[units] = 0;
        if (words[102] < GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(font, raster, dictionary, buffer)) {
            words[101] = words[101] + (current->length - column) + s4;
            return 1;
        }
    }
    words[101] = words[101] + (current->length - column) + next->length - 1;
    return 1;
}
}
