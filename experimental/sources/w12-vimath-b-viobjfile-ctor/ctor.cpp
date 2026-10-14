// Observed: __9VIObjFile at 0x010a60b0 is 40 bytes. With (this) it calls the base file
// constructor __6VIFile(this), then tail calls ResetState__9VIObjFile(this). The callee result
// is not used here; the routine has no stores of its own.
// Inferred: the VIObjFile constructor runs the VIFile constructor and then resets its own state.
// Unknown: the object size and whether the original returned a value to its callers.
extern "C" {
void __6VIFile(void* self);
void ResetState__9VIObjFile(void* self);

void __9VIObjFile(void* self) {
    __6VIFile(self);
    ResetState__9VIObjFile(self);
}
}
