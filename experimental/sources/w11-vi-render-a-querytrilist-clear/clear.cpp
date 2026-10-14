// Observed: Clear__14VIQueryTriList at 0x010d1188 is 68 bytes. If the word at
// offset 8 is nonzero it passes that word to __builtin_vec_delete and then stores
// zero there. It always stores zero at offsets 4 and 0 and returns 0.
// Inferred: offset 8 is an array allocated with vector new (see Allocate); 0 and
// 4 are a used count and a capacity. Unknown: which of those two each is.
extern "C" {
void __builtin_vec_delete(void* pointer);

int Clear__14VIQueryTriList(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned int& buffer = *reinterpret_cast<unsigned int*>(bytes + 8);
    if (buffer != 0) {
        __builtin_vec_delete(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(buffer)));
        buffer = 0;
    }
    *reinterpret_cast<int*>(bytes + 4) = 0;
    *reinterpret_cast<int*>(bytes + 0) = 0;
    return 0;
}
}
