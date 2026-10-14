// Observed: RemoveActor__7VIScenei at 0x01143e58, 304 bytes, in the pinned SLUS-20565 image. Arguments: the scene (S) and a 32-bit value A that the code uses as an address of an actor
// record (the mangled name says int). The word at A+152 is the head of a chain of occupancy nodes (size unknown), with 0xffffffff meaning none. Each node N holds a signed halfword
// I at +0, a signed halfword J at +2, a next-in-actor-chain pointer at +8, a previous pointer at +16 and a next pointer at +20 (the latter two chain it inside one cell).
// The cell of N is: base = word at S+80; element = base + 592*I; cell = (word at element+96) + 120*J. For each node of A's chain, in order:
//  1. If N's previous pointer is not -1, previous+20 receives N+20; otherwise the cell word at +108 receives N+20.
//  2. If N+20 (read again) is not -1, that node's +16 receives N+16; otherwise the cell word at +104 receives N+16.
//  3. Erase__t6VIList1ZQ27VIScene12VISceneOccupi(S+984, N) runs; a negative result returns -1 at once, leaving A's chain head as it is and the walk unfinished.
//  The next node is the actor-chain pointer at N+8, read before the erase.
// When the chain is exhausted (or was empty) the words at A+152 and A+156 are both set to -1 and 0 is returned.
// Inferred: the nodes are the elements of the scene-occupant list (the Erase instantiation name ends in VISceneOccup) linking an actor into the cells it overlaps, keeping a head word (+108) and a tail word (+104) per cell; the erased nodes come from a list at S+984.
// Unknown: the element and cell record layouts beyond the offsets used, the meaning of I and J, and what A+156 holds.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Erase__t6VIList1ZQ27VIScene12VISceneOccupi(unsigned char* list, unsigned char* node);

int RemoveActor__7VIScenei(unsigned char* self, int actorWord) {
    unsigned char* actor = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(actorWord));
    unsigned node = *reinterpret_cast<unsigned*>(actor + 152);
    while (node != 0xffffffffu) {
        unsigned char* n = NORRATH_WORD_TO_POINTER(node);
        const int i = *reinterpret_cast<short*>(n);
        const unsigned nextInActor = *reinterpret_cast<unsigned*>(n + 8);
        const unsigned previous = *reinterpret_cast<unsigned*>(n + 16);
        const int j = *reinterpret_cast<short*>(n + 2);
        const unsigned offset = static_cast<unsigned>(i) * 592u;
        if (previous != 0xffffffffu) {
            *reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(previous) + 20) = *reinterpret_cast<unsigned*>(n + 20);
        } else {
            unsigned char* cells = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 80)) + offset + 96));
            *reinterpret_cast<unsigned*>(cells + 120 * j + 108) = *reinterpret_cast<unsigned*>(n + 20);
        }
        const unsigned following = *reinterpret_cast<unsigned*>(n + 20);
        if (following != 0xffffffffu) {
            *reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(following) + 16) = *reinterpret_cast<unsigned*>(n + 16);
        } else {
            unsigned char* cells = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 80)) + offset + 96));
            *reinterpret_cast<unsigned*>(cells + 120 * j + 104) = *reinterpret_cast<unsigned*>(n + 16);
        }
        if (Erase__t6VIList1ZQ27VIScene12VISceneOccupi(self + 984, n) < 0) return -1;
        node = nextInActor;
    }
    *reinterpret_cast<int*>(actor + 156) = -1;
    *reinterpret_cast<int*>(actor + 152) = -1;
    return 0;
}
}
