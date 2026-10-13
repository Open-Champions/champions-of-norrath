// Observed: Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem at 0x01213c18 is 168 bytes (a weak template instance) in
// the pinned SLUS-20565 image. Arguments: the vector and a pointer to an element. Offsets are bytes from the vector.
//  1. When the word at +0 equals the word at +4: the word at +4 is set to twice the word at +0, or to 1 when that is zero, and
//     Allocate__t8VIVector1Z18VISceneDisplayElem(vector) runs; a negative result returns -1 (the +4 store stays).
//  2. index = the word at +0 (read again after Allocate). The slot is the address in the word at +8 plus 12 * index. When that
//     sum is not zero, 12 bytes are copied from the element to the slot (an 8-byte unaligned load/store plus a 4-byte one).
//  3. The word at +0 is then read again (the copy may have written it) and stored back plus one; the result is the value read.
// Inferred: +0 is the count, +4 the capacity and +8 the element storage of a growable array of 12-byte elements; the template
// argument comes only from the mangled name.
// Unknown: the element's fields. Allocate is an import. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

struct Elem12 { unsigned a, b, c; };

extern "C" {
int Allocate__t8VIVector1Z18VISceneDisplayElem(void* vector);

int Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(void* vector, const void* element) {
    unsigned* w = static_cast<unsigned*>(vector);
    if (w[0] == w[1]) {
        w[1] = w[0] != 0 ? w[0] * 2 : 1;
        if (Allocate__t8VIVector1Z18VISceneDisplayElem(vector) < 0) return -1;
    }
    unsigned index = w[0];
    unsigned address = w[2] + 12 * index;
    if (address != 0) *reinterpret_cast<Elem12*>(at(address)) = *static_cast<const Elem12*>(element);
    index = w[0];
    w[0] = index + 1;
    return static_cast<int>(index);
}
}
