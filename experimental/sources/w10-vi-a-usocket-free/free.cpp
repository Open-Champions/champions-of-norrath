// Observed: VIFreeUSocketExApi__Fv at 0x01048df8 is 36 bytes and takes no
// argument. It loads the word at 0x011e5c18 (local symbol sMessagePool in the
// image) and passes it to __wrap_free. It does not clear the word.
// Inferred: the word is a heap pointer to a message pool allocated by the
// matching setup code. Unknown: that code, and the pool layout. The word's
// defining unit is not authored, so it stays an import here.
extern "C" {
extern void* sMessagePool;
void __wrap_free(void* pointer);

void VIFreeUSocketExApi__Fv() {
    __wrap_free(sMessagePool);
}
}
