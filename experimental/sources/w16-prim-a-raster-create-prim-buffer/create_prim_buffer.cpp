// Observed: CreatePrimBuffer__8VIRasteri at 0x011132e0 is 140 bytes. It allocates 112 bytes with
// __builtin_new and, if the block is nonnull, runs the VIPrimBuffer constructor on it. A null
// block returns -1. Otherwise it keeps the block pointer in a one-word local and calls
// Add__t6VIPool1ZP12VIPrimBufferRCP12VIPrimBuffer on the pool embedded at offset 19320 of the
// raster object, passing the address of that local. A negative result returns -1. Otherwise, if
// the second argument is nonzero, the word at offset 4 of the new buffer is incremented. The
// function returns the pool-add result on success. (Same shape as VICollide::CreateCollBuffer.)
// Inferred: the pool result is the slot index and the incremented word is a reference count the
// caller asks to take. Unknown: the pool layout, and how a failed Add releases the new buffer
// (the original does not release it).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned __builtin_new(unsigned size);
void __12VIPrimBuffer(unsigned buffer);
int Add__t6VIPool1ZP12VIPrimBufferRCP12VIPrimBuffer(unsigned char* pool, unsigned* element);

int CreatePrimBuffer__8VIRasteri(unsigned char* self, int take_reference) {
    unsigned buffer = __builtin_new(112);
    if (buffer)
        __12VIPrimBuffer(buffer);
    unsigned element = buffer;
    if (!element)
        return -1;
    int result = Add__t6VIPool1ZP12VIPrimBufferRCP12VIPrimBuffer(self + 19320, &element);
    if (result < 0)
        return -1;
    if (take_reference) {
        unsigned char* b = at(element);
        *reinterpret_cast<int*>(b + 4) += 1;
    }
    return result;
}
}
