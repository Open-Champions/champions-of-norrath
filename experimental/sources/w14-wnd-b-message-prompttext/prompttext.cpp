// Observed: PromptText__12VIWndMessagePCUsi at 0x010c1670 is 152 bytes. With (this, text, flag) the buffer is
// the 128-unit 16-bit array at this+1008. When flag is nonzero it is first filled by
// str8to16__8VIStringPUsUiPCc(buffer, 128, "|q") (a two-character string, 0x7c 0x71, at 0x011d4fe8); when
// flag is zero only its first unit is set to 0. Then strcat16__8VIStringPUsUiPCUs(buffer, 128, text) appends
// the text, and, when flag is nonzero, strcat16__8VIStringPUsUiPCc(buffer, 128, "|w") appends the two characters
// 0x7c 0x77 (0x011d4ff0). It returns the buffer address.
// Inferred: "|q" and "|w" look like opening and closing text-style markers wrapped round the prompt when flag
// is set; this is a reading of the two literals only.
// Unknown: what the markers mean to the font code, the parameter name of flag, and that the buffer is 128 units
// long beyond the size passed to the helpers.
extern "C" {
void str8to16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);

unsigned short* PromptText__12VIWndMessagePCUsi(void* self, const unsigned short* text, int flag) {
    unsigned short* buffer = reinterpret_cast<unsigned short*>(static_cast<char*>(self) + 1008);
    if (flag != 0) {
        str8to16__8VIStringPUsUiPCc(buffer, 128, "|q");
    } else {
        buffer[0] = 0;
    }
    strcat16__8VIStringPUsUiPCUs(buffer, 128, text);
    if (flag != 0) {
        strcat16__8VIStringPUsUiPCc(buffer, 128, "|w");
    }
    return buffer;
}
}
