// Observed: DestroyPool__t7VIArray1Z10VIZoneLeaf at 0x01215090, 76 bytes. The array is the only argument. Reads the word at +0 (the element
// count) and runs a counted loop whose body does nothing: no memory is written, nothing is called or freed.
// The result is 0. For a negative count the image's loop effectively runs for billions of passes before
// leaving; that case is not reproduced (the authored function returns at once).
// Inferred: the element type has a trivial destructor, so the per-element teardown left an empty loop; +0
// holds the element count. The read of +0 is kept as a volatile load because the image performs it.
// Unknown: whether the original template wrote a destructor call that the compiler removed.
extern "C" {
int DestroyPool__t7VIArray1Z10VIZoneLeaf(void* array) {
    (void)*static_cast<const volatile unsigned*>(array);
    return 0;
}
}
