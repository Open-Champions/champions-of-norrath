// Observed: memcardTitle__FPUs at 0x0025bdf8 is 128 bytes. It calls
// textGetString__FPc with the string "MemcardPS2" (at 0x00474e70), calls
// getSlotName__FiiPc(-1, -1, 0), and then calls
// usprintf__FPUsT0ffffffff(out, text, 137.0f, (float)slot, 0, 0, 0, 0, 0, 0)
// where text is the first result reinterpreted as the format pointer and slot
// is the second call's integer result converted to float. It returns out, its
// own argument.
// Inferred: the 137.0f and the converted slot are the two numeric arguments of
// the localized memory card title format; nothing in the image says what
// either number stands for.
// Unknown: the third argument of getSlotName__FiiPc (0 here), the meaning of the
// -1 arguments, and the text behind the localized key.

extern "C" const char* textGetString__FPc(const char* key);
extern "C" int getSlotName__FiiPc(int a, int b, char* c);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* out, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);

extern "C" unsigned short* memcardTitle__FPUs(unsigned short* out) {
    unsigned short* text = reinterpret_cast<unsigned short*>(
        const_cast<char*>(textGetString__FPc("MemcardPS2")));
    int slot = getSlotName__FiiPc(-1, -1, 0);
    usprintf__FPUsT0ffffffff(out, text, 137.0f, static_cast<float>(slot), 0.0f, 0.0f, 0.0f,
                             0.0f, 0.0f, 0.0f);
    return out;
}
