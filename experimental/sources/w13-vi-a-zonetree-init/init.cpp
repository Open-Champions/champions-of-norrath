// Observed: Init__10VIZoneTreeii at 0x011271c0 is 92 bytes. Arguments: tree (a0) and two
// ints (a1, a2). It calls Init__t7VIArray1Z10VIZoneNodei(tree, a1). If that returns a
// negative value the function returns -1 without a second call. Otherwise it calls
// Init__t7VIArray1Z10VIZoneLeafi(tree + 8, a2) and returns -1 if that is negative, else 0.
// Inferred: the tree holds two arrays, nodes at +0 and leaves at +8, sized by the two
// arguments (a1 nodes, a2 leaves).
// Unknown: the element sizes and whether the arguments are counts or capacities.
extern "C" {
int Init__t7VIArray1Z10VIZoneNodei(void* array, int count);
int Init__t7VIArray1Z10VIZoneLeafi(void* array, int count);

int Init__10VIZoneTreeii(unsigned char* tree, int nodes, int leaves) {
    if (Init__t7VIArray1Z10VIZoneNodei(tree, nodes) < 0) {
        return -1;
    }
    return Init__t7VIArray1Z10VIZoneLeafi(tree + 8, leaves) < 0 ? -1 : 0;
}
}
