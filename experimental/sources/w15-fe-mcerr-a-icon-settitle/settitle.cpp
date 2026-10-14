// Observed: SetTitle__16VIIconDefinitionPCc at 0x01065c48 is 316 bytes. With (this, text):
//  1. it measures text with strlen and scans it for the byte 10 (newline). The first newline gives split =
//     its index; a second newline returns -1 at once. With no newline split stays -1.
//  2. a split index above 16 returns -1.
//  3. it copies text with strcpy into a local 48-byte area (before the length checks below, so a long text
//     overruns that area in the original; the authored copy keeps the same order and size).
//  4. with a newline present, a text of 34 or more bytes returns -1, otherwise the newline is removed by
//     shifting the rest of the copy left one byte and the copy is re-terminated; with no newline a text of 33
//     or more bytes returns -1.
//  5. it calls AsciiString2Sjis__16VIIconDefinitionPcPUs(this, copy, this + 192), then stores a 16-bit value
//     at +6: split * 2 when a newline was present, 32 otherwise. It returns 0.
// Inferred: this sets a one- or two-line save-icon title (the Shift-JIS conversion target at +192 and the
// 16-bit line-break position at +6 follow the icon-header layout from the card format); 16 characters
// per line and 32 in total are the limits the checks above imply.
// Unknown: the exact meaning of the value 32 at +6, the field names, and whether the stack overrun is a
// real hazard (callers pass short constant titles as far as seen).
extern "C" {
unsigned long strlen(const char* text);
char* strcpy(char* dest, const char* source);
void AsciiString2Sjis__16VIIconDefinitionPcPUs(void* self, char* text, unsigned short* out);

int SetTitle__16VIIconDefinitionPCc(void* self, const char* text) {
    char copy[48];
    int newlines = 0;
    int split = -1;
    const int length = static_cast<int>(strlen(text));
    for (int i = 0; i < length; ++i) {
        if (text[i] == 10) {
            ++newlines;
            if (newlines >= 2) return -1;
            split = i;
        }
    }
    if (split >= 17) return -1;
    strcpy(copy, text);
    if (newlines != 0) {
        if (length >= 34) return -1;
        for (int i = split; i < length; ++i) copy[i] = copy[i + 1];
        copy[length - 1] = 0;
    } else if (length >= 33) {
        return -1;
    }
    unsigned char* bytes = static_cast<unsigned char*>(self);
    AsciiString2Sjis__16VIIconDefinitionPcPUs(self, copy, reinterpret_cast<unsigned short*>(bytes + 192));
    *reinterpret_cast<unsigned short*>(bytes + 6) = newlines != 0 ? split * 2 : 32;
    return 0;
}
}
