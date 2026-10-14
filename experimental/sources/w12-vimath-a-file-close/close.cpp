// Observed: Close__6VIFile at 0x010c9fc8 is 192 bytes. The object (a0) has a signed word
// at +0 (when it is negative the first block below is skipped).
// 1. If word[+0] >= 0: BufferFlush__6VIFile(this); printf of the 16-character message
//    "VIEEFile: Close" followed by a newline; then, when bit 0x40 of word[+0] is set,
//    IsAsynchComplete__6VIFile(this) is called repeatedly until it returns nonzero. Word[+0]
//    is then set to -1.
// 2. Zero is stored at +4, +8, then 8 bytes at +16 and 8 bytes at +24. If the pointer at
//    +184 is nonzero it is released with __builtin_vec_delete and the words at +184 and
//    +188 are cleared.
// 3. 8 bytes at +32, 8 bytes at +40, the words at +48 and +52 are zeroed. If the pointer at
//    +56 is nonzero it is released with __builtin_vec_delete and the word at +56 is cleared.
// 4. Zero is stored at +112, +60 and 8 bytes at +64. The function returns 0.
// Inferred: this is the base-class close for an (asynchronous capable) file; +0 is the
// handle or mode word (-1 meaning closed), +184 and +56 are owned buffers. The message text
// names a class, VIEEFile, which differs from the symbol's class. Unknown: member names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int printf(const char* format, ...);
void BufferFlush__6VIFile(void* file);
int IsAsynchComplete__6VIFile(void* file);
void __builtin_vec_delete(void* pointer);

int Close__6VIFile(unsigned char* self) {
    int* handle = reinterpret_cast<int*>(self);
    if (*handle >= 0) {
        BufferFlush__6VIFile(self);
        printf("VIEEFile: Close\n");
        if ((*handle & 0x40) != 0) {
            while (IsAsynchComplete__6VIFile(self) == 0) {
            }
        }
        *handle = -1;
    }
    typedef unsigned long long __attribute__((may_alias)) Quad;
    *reinterpret_cast<int*>(self + 4) = 0;
    *reinterpret_cast<int*>(self + 8) = 0;
    *reinterpret_cast<Quad*>(self + 16) = 0;
    void* first = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 184));
    *reinterpret_cast<Quad*>(self + 24) = 0;
    if (first != nullptr) {
        __builtin_vec_delete(first);
        *reinterpret_cast<int*>(self + 184) = 0;
        *reinterpret_cast<int*>(self + 188) = 0;
    }
    void* second = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 56));
    *reinterpret_cast<Quad*>(self + 32) = 0;
    *reinterpret_cast<Quad*>(self + 40) = 0;
    *reinterpret_cast<int*>(self + 48) = 0;
    *reinterpret_cast<int*>(self + 52) = 0;
    if (second != nullptr) {
        __builtin_vec_delete(second);
        *reinterpret_cast<int*>(self + 56) = 0;
    }
    *reinterpret_cast<int*>(self + 112) = 0;
    *reinterpret_cast<int*>(self + 60) = 0;
    *reinterpret_cast<Quad*>(self + 64) = 0;
    return 0;
}
}
