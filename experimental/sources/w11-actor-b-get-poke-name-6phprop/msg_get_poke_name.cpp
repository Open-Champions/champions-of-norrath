// Observed msg_getPokeName__6PhPropPUsi at 0x00154a10, 168 bytes. out is a UTF-16 text buffer; the int
// argument is not used. In order:
//   1. the word at object+260 is set to the global frameNm;
//   2. text = textGetString("PickUp") is appended to out (ustrcat__FPUsT0), then the one-character
//      string " " (ustrcat__FPUsPc);
//   3. the name that getName__4itemPQ24item11descriptioni(object+328, 1) gives is appended;
//   4. count is the signed halfword at object+332; when count >= 2 the text " x %d" is formatted with
//      count into a 208-byte stack buffer using sprintf and appended with ustrcat__FPUsPc.
// Returns 1. The "PickUp" literal is a text-table key; the item record at +328 is the one msg_poke
// passes to playerMaybeGetItem. Unknown: the meaning of the word at +260 (frameNm looks like a frame
// number). Nothing here ran on the EE.
extern "C" {
extern int frameNm;
unsigned short* textGetString__FPc(char* key);
void ustrcat__FPUsT0(unsigned short* destination, unsigned short* source);
void ustrcat__FPUsPc(unsigned short* destination, char* source);
unsigned short* getName__4itemPQ24item11descriptioni(void* description, int article);
int sprintf(char* destination, const char* format, ...);

int msg_getPokeName__6PhPropPUsi(void* self, unsigned short* out, int) {
    unsigned char* o = static_cast<unsigned char*>(self);
    *reinterpret_cast<int*>(o + 260) = frameNm;
    ustrcat__FPUsT0(out, textGetString__FPc(const_cast<char*>("PickUp")));
    ustrcat__FPUsPc(out, const_cast<char*>(" "));
    ustrcat__FPUsT0(out, getName__4itemPQ24item11descriptioni(o + 328, 1));
    const int count = *reinterpret_cast<short*>(o + 332);
    if (count >= 2) {
        char text[208];
        sprintf(text, " x %d", count);
        ustrcat__FPUsPc(out, text);
    }
    return 1;
}
}
