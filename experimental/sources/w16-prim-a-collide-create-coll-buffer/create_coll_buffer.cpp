// Observed: CreateCollBuffer__9VICollidei at 0x01096f40 is 140 bytes. It allocates 92 bytes with
// __builtin_new and, if the block is nonnull, runs the VICollBuffer constructor on it. A null
// block returns -1. Otherwise it keeps the block pointer in a one-word local and calls
// Add__t6VIPool1ZP12VICollBufferRCP12VICollBuffer on the pool embedded at offset 528 of the collision object,
// passing the address of that local. A negative result returns -1. Otherwise, if the second
// argument is nonzero, the word at offset 4 of the new buffer is incremented. The function
// returns the pool-add result (not zero) on success.
// Inferred: the pool result is the slot index of the new buffer, and the incremented word is a
// reference count the caller asks to take. Unknown: the pool's layout and how a failed Add
// releases the freshly built buffer (the original does not release it).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned __builtin_new(unsigned size);
void __12VICollBuffer(unsigned buffer);
int Add__t6VIPool1ZP12VICollBufferRCP12VICollBuffer(unsigned char* pool, unsigned* element);

int CreateCollBuffer__9VICollidei(unsigned char* self, int take_reference) {
    unsigned buffer = __builtin_new(92);
    if (buffer)
        __12VICollBuffer(buffer);
    unsigned element = buffer;
    if (!element)
        return -1;
    int result = Add__t6VIPool1ZP12VICollBufferRCP12VICollBuffer(self + 528, &element);
    if (result < 0)
        return -1;
    if (take_reference) {
        unsigned char* b = at(element);
        *reinterpret_cast<int*>(b + 4) += 1;
    }
    return result;
}
}
