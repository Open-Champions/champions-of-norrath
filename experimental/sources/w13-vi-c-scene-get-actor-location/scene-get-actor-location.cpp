// Observed: GetActorLocation__C7VIScenei at 0x01145730, 8 bytes. With the scene
// as this and the second argument as a value, it returns that value plus 68 and
// reads and writes nothing; the scene is not touched.
// Inferred: the mangled signature says the argument is an int, but the image
// treats it as an address (callers pass a record address) and the result is the
// address of an embedded location inside that record. The authored parameter
// and result are therefore pointers, which are the same 32-bit width here.
// Unknown: the record type, and whether the result is a three-float vector.
extern "C" {
const void* GetActorLocation__C7VIScenei(const void* scene, const void* actor) {
    return static_cast<const unsigned char*>(actor) + 68;
}
}
