// Observed: MC_ConvertTempPlayerToReal__Fi at 0x0025b3d0 is 128 bytes. The argument n selects
// record n of two arrays whose records are 23984 bytes apart: tempPlayerStruct (0x007e05e0)
// and player (0x00408600). It copies the 23984 bytes of tempPlayerStruct record n over
// player record n, in 32-byte steps with a 16-byte tail, then calls
// convertIndiciesToPointers__Fi(n). It returns nothing and does no bounds check.
// Inferred: the call turns the stored indices in the copied record back into pointers, from the
// callee's name; the same pairing is used by MC_CheckPlayerImportOrChange__Fv.
// Unknown: how many records either array holds.

extern "C" void convertIndiciesToPointers__Fi(int index);

namespace {
struct alignas(8) PlayerRecord {
    unsigned char bytes[23984];
};
}

extern "C" PlayerRecord tempPlayerStruct[];
extern "C" PlayerRecord player[];

extern "C" void MC_ConvertTempPlayerToReal__Fi(int n) {
    player[n] = tempPlayerStruct[n];
    convertIndiciesToPointers__Fi(n);
}
