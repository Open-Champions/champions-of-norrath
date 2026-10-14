// Observed: GetMode__6Camera at 0x0026fc60 is 8 bytes. Takes the camera object (a0) and returns the
// 32-bit word at offset 52; nothing is written and no call is made.
// Inferred: an accessor for a camera mode value; the return is treated as a plain int.
// Unknown: the set of mode values, whether the field is an enum, and the camera object size.
extern "C" {
int GetMode__6Camera(const unsigned char* camera) {
    return *reinterpret_cast<const int*>(camera + 52);
}
}
