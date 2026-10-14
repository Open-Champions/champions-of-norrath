// Observed: Init__11VIWorldTreeii at 0x01100238 is 92 bytes. With the tree as this and two ints
// (a, b): it calls Init__t7VIArray1Z11VIWorldNodei(this, a); a negative result is returned as
// -1. Otherwise it calls Init__t7VIArray1Z11VIWorldLeafi(this+8, b) and returns -1 if that is
// negative, else 0.
// Inferred: the tree is two arrays, nodes at +0 and leaves at +8, with node count a and leaf count b.
// Unknown: member names.
extern "C" {
int Init__t7VIArray1Z11VIWorldNodei(unsigned char* array, int count);
int Init__t7VIArray1Z11VIWorldLeafi(unsigned char* array, int count);

int Init__11VIWorldTreeii(unsigned char* self, int node_count, int leaf_count) {
    if (Init__t7VIArray1Z11VIWorldNodei(self, node_count) < 0) return -1;
    return Init__t7VIArray1Z11VIWorldLeafi(self + 8, leaf_count) < 0 ? -1 : 0;
}
}
