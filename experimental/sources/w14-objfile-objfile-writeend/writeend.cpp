// Observed: WriteEnd__9VIObjFile at 0x010a66d8, 28 bytes. Calls WriteEnd__9VIObjFileRUi(this, &local) with the address of a
// stack unsigned word that it never reads or initialises, and returns that call's result unchanged.
// Inferred: the one-argument form of the stream-end routine; the overload receives the reference as its second argument
// (the mangled name ends in a reference to unsigned int) and the value is discarded here.
// Unknown: what the callee stores into the word.
extern "C" {
int WriteEnd__9VIObjFileRUi(void* object, unsigned* out);

int WriteEnd__9VIObjFile(void* object) {
    unsigned discarded;
    return WriteEnd__9VIObjFileRUi(object, &discarded);
}
}
