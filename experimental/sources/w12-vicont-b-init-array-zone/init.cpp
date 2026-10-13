// Observed: Init__t7VIArray1Z6VIZonei at 0x0120f930, 92 bytes. Arguments are the array and a count.
//  1. Calls Clear__t7VIArray1Z6VIZone(array); a negative result returns -1 with nothing stored.
//  2. Stores the count at +0, calls Allocate__t7VIArray1Z6VIZone(array), and returns -1 when that is negative, else 0.
// Inferred: the word at +0 is the element count that Allocate consumes. The -1/0 result convention is only this code shape.
// The template argument comes only from the mangled name.
extern "C" {
int Clear__t7VIArray1Z6VIZone(void* array);
int Allocate__t7VIArray1Z6VIZone(void* array);

int Init__t7VIArray1Z6VIZonei(void* array, int count) {
    if (Clear__t7VIArray1Z6VIZone(array) < 0) return -1;
    *static_cast<int*>(array) = count;
    return Allocate__t7VIArray1Z6VIZone(array) < 0 ? -1 : 0;
}
}
