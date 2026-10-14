// arList at 0x00486914, four .sbss bytes, next arListBack.
// The image value is zero. Under -mlong32 this pointer is one word.
// animInit stores animResults here later.
extern "C" {
unsigned char* arList;
}
