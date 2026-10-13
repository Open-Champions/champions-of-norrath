// Observed: Init__9VIClipperi at 0x0108c1b8 is 92 bytes. Arguments: the object (a0) and
// an int count (a1). It calls Init__t7VIArray1Z7VIVect3i(this, count) and, only if that
// returned a value >= 0, calls it again with (this + 8, count). It returns -1 when the
// first call returned a negative value, otherwise -1 when the second call returned a
// negative value, otherwise 0. Nothing else is read or written.
// Inferred: the clipper owns two arrays of VIVect3 at offsets 0 and 8, each sized to
// the count (the template name is the mangled Init of VIArray<VIVect3>), and a negative
// result means allocation failure. Unknown: the class layout beyond those two members.
extern "C" {
int Init__t7VIArray1Z7VIVect3i(void* array, int count);

int Init__9VIClipperi(unsigned char* self, int count) {
    if (Init__t7VIArray1Z7VIVect3i(self, count) < 0) return -1;
    if (Init__t7VIArray1Z7VIVect3i(self + 8, count) < 0) return -1;
    return 0;
}
}
