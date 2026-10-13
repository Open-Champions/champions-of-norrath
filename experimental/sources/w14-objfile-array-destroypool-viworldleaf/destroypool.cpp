// Observed: DestroyPool__t7VIArray1Z11VIWorldLeaf at 0x012101d0, 76 bytes. Reads the word at +0 of the object (the element count) and loops
// over that many elements doing nothing to memory (the loop body is empty, so each element's destruction is a no-op), then returns 0. It
// writes no memory and calls nothing. For a negative count the image's loop effectively runs for billions of iterations before
// leaving; that case is not reproduced (the authored function returns at once) and callers are not expected to reach it.
// Inferred: the element destructor of this container's element type is trivial; +0 holds the element count.
// Unknown: whether the original template wrote a destructor call that the compiler removed.
extern "C" {
int DestroyPool__t7VIArray1Z11VIWorldLeaf(void* array) {
    (void)array;
    return 0;
}
}
