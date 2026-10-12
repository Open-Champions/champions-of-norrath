// Observed resetTextBody__Fv at 0x001d3940, 44 bytes. It clears the 2048 bytes
// of textBody (0x0042a500) with memset and stores zero in the word textBodyLen
// (0x00486b68). Nothing is returned.
// Unknown: element type of textBody beyond its 2048-byte extent (the next
// image symbol starts 0x800 later); it is declared as 16-bit units here.
extern "C" void* memset(void*, int, __SIZE_TYPE__);
extern "C" unsigned short textBody[1024];
extern "C" int textBodyLen;

extern "C" void resetTextBody__Fv() {
    textBodyLen = 0;
    memset(textBody, 0, sizeof textBody);
}
