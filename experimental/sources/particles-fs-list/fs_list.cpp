// fsList at 0x0048690c, four .sbss bytes, next fsListBack.
// The image value is zero. Under -mlong32 this pointer is one word.
// animInit stores frameStates here later.
extern "C" {
unsigned char* fsList;
}
