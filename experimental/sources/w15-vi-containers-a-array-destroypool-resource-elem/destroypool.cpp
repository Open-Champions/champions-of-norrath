// Observed: DestroyPool__t7VIArray1Z14VIResourceElem at 0x01206ef0, 76 bytes. It reads the word at +0 of the object (the element
// count n) and, for n != 0, runs an index loop from 0 through n-1 whose body is empty, then returns 0. It writes no memory and calls
// nothing. For a negative count the image's loop runs for about four billion iterations before it stops; that case is not reproduced
// (the authored function returns at once) and callers are not expected to reach it.
// Inferred: the element destructor of this container's element type is trivial, so the loop over the elements does nothing; +0 holds the
// element count.
// Unknown: whether the original template wrote a destructor call that the compiler removed.
extern "C" {
int DestroyPool__t7VIArray1Z14VIResourceElem(void* array) {
    (void)array;
    return 0;
}
}
