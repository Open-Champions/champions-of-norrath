// Observed: Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm at 0x010a4968 is 628
// bytes. Arguments: the CSF object (a0), a file (a1), an int reference (a2, count out) and
// a pointer to a pointer (a3, segment table out). The object's word at +0 is a callback
// pointer (zero for none) and its word at +4 is the callback's argument; each time the
// function says "callback" below it calls object[+0](object[+4]) when the pointer is
// nonzero.
// 1. Read__6VIFilePvi(file, header, 40); a negative result returns -1. The 40-byte header
//    is: u32 magic at +0, u32 count at +4, 8-byte values at +8, +16 and +24, u32 at +32.
//    When the global VIEndian is not 1 the fields are byte-swapped with
//    VIBufferSwap__FiPUc(size, address) in the order +0 (4), +4 (4), +8 (8), +16 (8), +24
//    (8), +32 (4).
// 2. The magic must be 0x46534543, otherwise -1. Callback. Then *count = header count; the
//    segment table is allocated with __builtin_vec_new(count * 16) and its address stored
//    in *table; a null result returns -1. A scratch buffer of header[+32] bytes is then
//    allocated the same way (null returns -1).
// 3. Seek__6VIFilelQ26VIFile10SeekOrigin(file, the 64-bit value at header+24, 1); a
//    negative result returns -1. (The value is passed as the full 64-bit register.)
// 4. For each of *count chunks (none when the count is not positive): read an 8-byte chunk
//    header (u32 compressed size, u32 uncompressed size; swapped as two 4-byte fields when
//    VIEndian is not 1), read the compressed size bytes into the scratch buffer, callback,
//    store the sign-extended uncompressed size as a 64-bit value at table[i*16 + 0],
//    allocate that many bytes with __builtin_vec_new and store the pointer at table[i*16 +
//    8] (null returns -1 after the store), then call uncompress(dest, &length, scratch,
//    compressed size) with length preset to the uncompressed size. A nonzero result, or a
//    resulting length different from the uncompressed size, returns -1. Callback again.
//    Every failed read also returns -1.
// 5. After the last chunk, the scratch buffer is released with __builtin_vec_delete and the
//    function returns 0. Buffers are not released on any failure path.
// Inferred: this decompresses a container of independently compressed (zlib-style) chunks
// into separately allocated memory segments (16-byte VIFile::MemSegm records: 64-bit size,
// then data pointer); the callback is a progress hook. Unknown: member names, the meaning of
// header fields +8 and +16, and what the 'segment' pointer type contains beyond +0/+8.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
extern "C" void norrath_host_call(unsigned function, unsigned argument);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_POINTER_TO_WORD(pointer) norrath_host_address(pointer)
#define NORRATH_CALL_WORD(function, argument) norrath_host_call(function, argument)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_POINTER_TO_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#define NORRATH_CALL_WORD(function, argument) (reinterpret_cast<void (*)(unsigned)>(static_cast<__UINTPTR_TYPE__>(function))(argument))
#endif

extern "C" {
extern int VIEndian;
int Read__6VIFilePvi(void* file, void* buffer, int size);
int Seek__6VIFilelQ26VIFile10SeekOrigin(void* file, long long offset, int origin);
void VIBufferSwap__FiPUc(int size, unsigned char* data);
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
int uncompress(void* dest, unsigned* dest_length, const void* source, unsigned source_length);

static inline void callback(unsigned char* self) {
    unsigned function = *reinterpret_cast<unsigned*>(self);
    if (function != 0) NORRATH_CALL_WORD(function, *reinterpret_cast<unsigned*>(self + 4));
}

int Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(unsigned char* self, void* file, int* count, unsigned* table) {
    alignas(16) unsigned char header[48];
    alignas(16) unsigned char chunk[16];
    unsigned length;
    if (Read__6VIFilePvi(file, header, 40) < 0) return -1;
    int endian = VIEndian;
    if (endian != 1) {
        VIBufferSwap__FiPUc(4, header);
        VIBufferSwap__FiPUc(4, header + 4);
        VIBufferSwap__FiPUc(8, header + 8);
        VIBufferSwap__FiPUc(8, header + 16);
        VIBufferSwap__FiPUc(8, header + 24);
        VIBufferSwap__FiPUc(4, header + 32);
    }
    if (*reinterpret_cast<unsigned*>(header) != 0x46534543u) return -1;
    callback(self);
    *count = *reinterpret_cast<int*>(header + 4);
    void* segments = __builtin_vec_new(static_cast<unsigned>(*reinterpret_cast<int*>(header + 4)) << 4);
    if (segments == nullptr) return -1;
    *table = NORRATH_POINTER_TO_WORD(static_cast<unsigned char*>(segments));
    void* scratch = __builtin_vec_new(*reinterpret_cast<unsigned*>(header + 32));
    if (scratch == nullptr) return -1;
    if (Seek__6VIFilelQ26VIFile10SeekOrigin(file, *reinterpret_cast<long long*>(header + 24), 1) < 0) return -1;
    int total = *count;
    if (total > 0) {
        int index = 0;
        do {
            if (Read__6VIFilePvi(file, chunk, 8) < 0) return -1;
            if (endian != 1) {
                VIBufferSwap__FiPUc(4, chunk);
                VIBufferSwap__FiPUc(4, chunk + 4);
            }
            if (Read__6VIFilePvi(file, scratch, *reinterpret_cast<int*>(chunk)) < 0) return -1;
            callback(self);
            unsigned char* records = NORRATH_WORD_TO_POINTER(*table);
            int original = *reinterpret_cast<int*>(chunk + 4);
            *reinterpret_cast<long long*>(records + index * 16) = original;
            void* data = __builtin_vec_new(static_cast<unsigned>(original));
            records = NORRATH_WORD_TO_POINTER(*table);
            *reinterpret_cast<unsigned*>(records + index * 16 + 8) = NORRATH_POINTER_TO_WORD(static_cast<unsigned char*>(data));
            if (data == nullptr) return -1;
            length = static_cast<unsigned>(original);
            if (uncompress(data, &length, scratch, *reinterpret_cast<unsigned*>(chunk)) != 0) return -1;
            if (length != static_cast<unsigned>(*reinterpret_cast<int*>(chunk + 4))) return -1;
            callback(self);
            ++index;
        } while (index < *count);
    }
    if (scratch != nullptr) __builtin_vec_delete(scratch);
    return 0;
}
}
