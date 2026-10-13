// Observed: Init__t7VIArray1Z11VIWorldNodei at 0x0120ff68 is 92 bytes and returns an int. With (this, count) it calls Clear__t7VIArray1Z11VIWorldNode(this);
// if that result is negative it returns -1. Otherwise it stores count at this+0, calls Allocate__t7VIArray1Z11VIWorldNode(this)
// and returns -1 when that result is negative, else 0.
// Inferred: the word at this+0 is the element count that Allocate consumes; -1/0 is the failure/success
// convention of these container routines (only this code shape is evidence).
// Unknown: the other fields of the container and the value ranges of Clear and Allocate results.
extern "C" {
int Clear__t7VIArray1Z11VIWorldNode(void* self);
int Allocate__t7VIArray1Z11VIWorldNode(void* self);

int Init__t7VIArray1Z11VIWorldNodei(void* self, int count) {
    if (Clear__t7VIArray1Z11VIWorldNode(self) < 0) return -1;
    static_cast<int*>(self)[0] = count;
    return Allocate__t7VIArray1Z11VIWorldNode(self) < 0 ? -1 : 0;
}
}
