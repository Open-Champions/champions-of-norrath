// Observed msg_getPokeName__9AutoChestPUsi at 0x224bc0, 168 bytes. The int is not read.
// If the word at object+216 is zero, copies the text-table entry for the key "OpenChest"
// into the 16-bit string buffer (textGetString, then ustrcpy). Otherwise, if the signed
// halfword at +220 is positive, formats the text-table entry for "PickUpGold" into a
// local 16-bit buffer with usprintf, passing that halfword converted to float followed by
// seven 0.0 values, and copies the result into the string buffer. Always returns 1.
// The keys sit at 0x470d68 and 0x470d78 in the image's string area; this source holds
// its own copies. The local buffer is 400 bytes; the image's frame leaves that much
// room below its saved registers, but the real limit of the formatted text is unknown.
// Unknown: what the +216 word and the +220 halfword mean, and the destination size.

extern "C" {
unsigned short* textGetString__FPc(const char* key);
void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source);
void usprintf__FPUsT0ffffffff(unsigned short* destination, unsigned short* format, float a, float b, float c,
                              float d, float e, float f, float g, float h);
}

typedef unsigned Word __attribute__((may_alias));
typedef short Half __attribute__((may_alias));

extern "C" int msg_getPokeName__9AutoChestPUsi(void* self, unsigned short* name, int index) {
    (void)index;
    const unsigned char* object = static_cast<const unsigned char*>(self);
    if (*reinterpret_cast<const Word*>(object + 216) == 0) {
        ustrcpy__FPUsT0(name, textGetString__FPc("OpenChest"));
        return 1;
    }
    short amount = *reinterpret_cast<const Half*>(object + 220);
    if (amount > 0) {
        unsigned short text[200];
        usprintf__FPUsT0ffffffff(text, textGetString__FPc("PickUpGold"), static_cast<float>(amount), 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 0.0f);
        ustrcpy__FPUsT0(name, text);
    }
    return 1;
}
