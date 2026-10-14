// Observed: CreateSurface__8VIRasteri at 0x01113730 is 152 bytes in the pinned SLUS-20565 image. It allocates 316 bytes with
// __builtin_new and, when the allocation is nonnull, runs the VISurface constructor (__9VISurface) on it. A null allocation returns -1.
// Otherwise it adds the pointer to the pool at +19380 of the raster object with Add__t6VIPool1ZP9VISurfaceRCP9VISurface, which returns
// an index; a negative index returns -1. On success, if the argument is nonzero it adds one to the word at +4 of the new surface, then it
// stores the index at +20 of the surface and returns the index.
// Inferred: the pool at +19380 is the surface table whose entries Surface__8VIRasteri and ShareSurface__8VIRasteri read (its entry base
// pointer sits at +19388); the +4 word is a reference count and the argument asks for one initial share; +20 records the pool index.
// Unknown: the pool object layout, the constructor's effects, and the failure state of a surface whose Add failed (it is not freed).
// Nothing here ran on the EE.
extern "C" {
unsigned char* __builtin_new(unsigned size);
void __9VISurface(unsigned char* self);
int Add__t6VIPool1ZP9VISurfaceRCP9VISurface(unsigned char* pool, unsigned char** item);
int CreateSurface__8VIRasteri(unsigned char* self, int share) {
    unsigned char* surface = __builtin_new(316);
    if (surface) __9VISurface(surface);
    unsigned char* slot = surface;
    if (!slot) return -1;
    int index = Add__t6VIPool1ZP9VISurfaceRCP9VISurface(self + 19380, &slot);
    if (index < 0) return -1;
    if (share) *reinterpret_cast<int*>(slot + 4) += 1;
    *reinterpret_cast<int*>(slot + 20) = index;
    return index;
}
}
