// Observed msg_getPokeName__8ItemPropPUsi at 0x155920, 168 bytes. The integer
// argument is not read.
// Stores the word frameNm (0x4844a4) at object+260. Then appends to the
// UTF-16 buffer in order: textGetString("PickUp"), the one-character string " "
// (0x483b88), and item::getName(object+328, 1). When the signed halfword at
// object+332 is 2 or more, " x %d" (0x483b90) is formatted with that value
// into a local buffer and appended as well. Returns 1.
// Unknown: the size of the local buffer; the frame holds at least 208 bytes
// below the saved registers, and 208 is used here.

#include "../inventory-items-object/object.hpp"

extern "C" {
extern int frameNm;
unsigned short* textGetString__FPc(const char* key);
void ustrcat__FPUsT0(unsigned short* destination, const unsigned short* source);
void ustrcat__FPUsPc(unsigned short* destination, const char* source);
unsigned short* getName__4itemPQ24item11descriptioni(void* description, int mode);
int sprintf(char* buffer, const char* format, ...);
}

extern "C" int msg_getPokeName__8ItemPropPUsi(void* object_storage, unsigned short* text, int) {
    using namespace inventory_object;
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 260, static_cast<unsigned>(frameNm));
    ustrcat__FPUsT0(text, textGetString__FPc("PickUp"));
    ustrcat__FPUsPc(text, " ");
    ustrcat__FPUsT0(text, getName__4itemPQ24item11descriptioni(object + 328, 1));
    const int count = static_cast<short>(unsigned(object[332]) | (unsigned(object[333]) << 8));
    if (count >= 2) {
        char number[208];
        sprintf(number, " x %d", count);
        ustrcat__FPUsPc(text, number);
    }
    return 1;
}
