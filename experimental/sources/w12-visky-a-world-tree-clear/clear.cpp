// Observed: Clear__11VIWorldTree at 0x01100298 is 80 bytes. With the tree as this it calls
// Clear__t7VIArray1Z11VIWorldNode(this); a negative result is returned as -1. Otherwise it calls
// Clear__t7VIArray1Z11VIWorldLeaf(this+8) and returns -1 if that is negative, else 0.
// Inferred: the counterpart of Init, for the node array at +0 and the leaf array at +8.
// Unknown: member names.
extern "C" {
int Clear__t7VIArray1Z11VIWorldNode(unsigned char* array);
int Clear__t7VIArray1Z11VIWorldLeaf(unsigned char* array);

int Clear__11VIWorldTree(unsigned char* self) {
    if (Clear__t7VIArray1Z11VIWorldNode(self) < 0) return -1;
    return Clear__t7VIArray1Z11VIWorldLeaf(self + 8) < 0 ? -1 : 0;
}
}
