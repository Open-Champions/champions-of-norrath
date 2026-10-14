// Host contract for __builtin_vec_new. Not an EE plan input.
#include "builtin_vec_new.cpp"
static unsigned seen_size = 0xdeadu; static int calls; static char block[4];
extern "C" void* __builtin_new(unsigned size) { seen_size = size; ++calls; return size == 0 ? 0 : block; }
int main() {
    if (__builtin_vec_new(24) != block || seen_size != 24 || calls != 1) return 1;
    if (__builtin_vec_new(0) != 0 || seen_size != 0 || calls != 2) return 2;   // size is passed on unchanged
    if (__builtin_vec_new(0xffffffffu) != block || seen_size != 0xffffffffu) return 3;
    return 0;
}
