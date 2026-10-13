// Observed: Init__t7VIArray1Zii at 0x011f50a0 is 92 bytes and returns an int. With (this, count)
// it calls Clear__t7VIArray1Zi(this); if that result is negative it returns -1. Otherwise it stores
// count at this+0, calls Allocate__t7VIArray1Zi(this) and returns -1 when that result is negative,
// else 0.
// Inferred: the word at this+0 is the element count that Allocate consumes; -1/0 is the
// failure/success convention of these container routines (only this code shape is evidence).
// Unknown: the other fields of the container and the exact value ranges of Clear/Allocate results.
struct Container {
    int count;
};

extern "C" {
int Clear__t7VIArray1Zi(Container* self);
int Allocate__t7VIArray1Zi(Container* self);

int Init__t7VIArray1Zii(Container* self, int count) {
    if (Clear__t7VIArray1Zi(self) < 0) return -1;
    self->count = count;
    return Allocate__t7VIArray1Zi(self) < 0 ? -1 : 0;
}
}
