// Observed msg_getPokeName__5ChestPUsi at 0x0021dc80, 176 bytes. The int argument is not read.
// Always returns 1.
//   - word at object+132 is zero: text-table entry "OpenChest" is copied into the 16-bit
//     buffer (textGetString, then ustrcpy);
//   - otherwise, when the signed halfword at object+76 is greater than zero: the text-table
//     entry "PickUpGold" is formatted with usprintf into a local 16-bit buffer, passing the
//     halfword converted to float as the first float argument and 0.0 for the other seven, and
//     that buffer is copied into the caller's buffer with ustrcpy;
//   - otherwise nothing is written.
// The key texts sit in the image's shared string area; this source holds its own copies.
// Unknown: what the +76 amount and the +132 word are, and the size of the destination
// buffer. The local format buffer here is 200 halfwords; the original frame leaves at most
// 400 bytes for it.
extern "C" {
unsigned short* textGetString__FPc(const char* key);
void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source);
void usprintf__FPUsT0ffffffff(unsigned short* destination, const unsigned short* format, float a, float b,
                              float c, float d, float e, float f, float g, float h);
}

extern "C" int msg_getPokeName__5ChestPUsi(void* self, unsigned short* name, int) {
    typedef unsigned UWord __attribute__((may_alias));
    typedef short Half __attribute__((may_alias));
    const unsigned char* object = static_cast<const unsigned char*>(self);
    if (*reinterpret_cast<const UWord*>(object + 132) == 0u) {
        ustrcpy__FPUsT0(name, textGetString__FPc("OpenChest"));
        return 1;
    }
    short amount = *reinterpret_cast<const Half*>(object + 76);
    if (amount > 0) {
        unsigned short text[200];
        usprintf__FPUsT0ffffffff(text, textGetString__FPc("PickUpGold"), static_cast<float>(amount), 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        ustrcpy__FPUsT0(name, text);
    }
    return 1;
}
