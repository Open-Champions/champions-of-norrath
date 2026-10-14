// Observed: __19VIFlatFileStructure at 0x01004fe8, 132 bytes. Offsets are bytes from the object. In order: +12 = 2, +8 = -1,
// +16 = the address of the class table of VIFlatFileDirectory (_vt$19VIFlatFileDirectory), +0 = 0, +4 = 0; then it calls
// __t6VIList1ZP10VIFlatFile (a constructor) with the address object+24; then +20 = 0. Finally, if the word at +0 is nonzero it
// is passed to __wrap_free, and in every case +0 receives the result of __wrap_strdup applied to the one-character constant
// string "/". Nothing is returned.
// Inferred: a directory-like object with a root-path string at +0 (set to "/" here; the word is cleared first, so the free call
// is reachable only if the list constructor stored something there), a list of VIFlatFile pointers at +24, and the class-table pointer at +16 shared with the
// base class VIFlatFileDirectory. The meaning of the words at +4, +8, +12 and +20 is not established.
// Unknown: the object size, and the meaning of the values 2 and -1.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
static inline unsigned address_of(const void* pointer) { return norrath_host_address(pointer); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
static inline unsigned address_of(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(pointer)); }
#endif

extern unsigned char _vt$19VIFlatFileDirectory[] __asm__("_vt$19VIFlatFileDirectory");

extern "C" {
void __t6VIList1ZP10VIFlatFile(void* list);
void __wrap_free(void* block);
char* __wrap_strdup(const char* text);

void __19VIFlatFileStructure(void* self) {
    unsigned* words = static_cast<unsigned*>(self);
    words[3] = 2;
    words[2] = static_cast<unsigned>(-1);
    words[4] = address_of(_vt$19VIFlatFileDirectory);
    words[0] = 0;
    words[1] = 0;
    __t6VIList1ZP10VIFlatFile(words + 6);
    words[5] = 0;
    if (words[0] != 0) __wrap_free(at(words[0]));
    words[0] = address_of(__wrap_strdup("/"));
}
}
