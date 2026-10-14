// Observed: AsciiString2Sjis__16VIIconDefinitionPcPUs at 0x01065f30 is 172 bytes. Arguments: the icon definition object,
// a text pointer and an output pointer of 16-bit units. It measures the text with strlen, then for each character in
// order calls Ascii2Sjis__16VIIconDefinitionc(this, character) (the character read as a signed byte), ORs
// its low byte shifted up by eight with the whole result shifted down by eight (a byte swap for a 16-bit result; bits 16 to
// 23 of a wider result also reach the high byte) and stores the low 16 bits at the next output position. It stores a zero unit after
// the last one and returns the address of that zero unit.
// Inferred: converts a plain character string to the byte-swapped Shift-JIS code units the icon text uses; the swap makes
// the units match the order a following consumer expects.
// Unknown: the name of that consumer, and the return type (a pointer is assumed from the address arithmetic).
extern "C" {
unsigned Ascii2Sjis__16VIIconDefinitionc(void* self, char character);
unsigned strlen(const char* text);

unsigned short* AsciiString2Sjis__16VIIconDefinitionPcPUs(void* self, const char* text, unsigned short* output) {
    const int length = static_cast<int>(strlen(text));
    int index = 0;
    while (index < length) {
        const unsigned code = Ascii2Sjis__16VIIconDefinitionc(self, text[index]);
        output[index] = static_cast<unsigned short>(((code & 0xffu) << 8) | (code >> 8));
        index += 1;
    }
    output[index] = 0;
    return output + index;
}
}
