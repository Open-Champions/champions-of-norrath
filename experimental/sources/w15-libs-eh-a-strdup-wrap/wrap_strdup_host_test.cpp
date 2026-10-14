// Host check only; not an EE build input. Build the source with
// -D__wrap_strdup=w15_wrap_strdup -Dstrdup=w15_t_strdup -Dstrlen=w15_t_strlen -Dstrcpy=w15_t_strcpy (the source alone; this file is compiled without them)
// so the callees are the stubs below.
#include <cstdio>
#include <initializer_list>
#include <cstring>
extern "C" {
unsigned char enableMallocWrapper;
}
static int dup_calls, alloc_calls, bytes_seen, copied; static char block[64];
extern "C" char* w15_t_strdup(const char* t) { ++dup_calls; return const_cast<char*>(t); }
extern "C" unsigned w15_t_strlen(const char* t) { return std::strlen(t); }
extern "C" char* w15_t_strcpy(char* d, const char* s) { ++copied; return std::strcpy(d, s); }
extern "C" void* memAlloc__FiPc(int n, char* tag) { ++alloc_calls; bytes_seen = n; return tag && !*tag ? block : nullptr; }
extern "C" char* w15_wrap_strdup(const char* t);
int main() {
    const char* text = "hello";
    enableMallocWrapper = 0;
    if (w15_wrap_strdup(text) != text || dup_calls != 1 || alloc_calls != 0) return 1;
    for (unsigned flag : {1u, 0x80u, 255u}) {
        enableMallocWrapper = static_cast<unsigned char>(flag);
        std::memset(block, 'z', sizeof block);
        char* r = w15_wrap_strdup(text);
        if (r != block || std::strcmp(block, "hello") != 0) return 2;
        if (bytes_seen != 6 || dup_calls != 1) return 3;
    }
    if (alloc_calls != 3 || copied != 3) return 4;
    enableMallocWrapper = 1;
    w15_wrap_strdup("");
    if (bytes_seen != 1) return 5;
    std::puts("wrap_strdup ok");
    return 0;
}
