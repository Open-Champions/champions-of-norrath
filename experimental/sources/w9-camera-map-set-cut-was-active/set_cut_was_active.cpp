// Observed SetCutWasActive__6Camera at 0x0026fc30, 16 bytes.
// Stores the byte value 5 at camera+365. Nothing is read or returned. What the
// value 5 counts is not established here. Nothing here was executed on the EE.
extern "C" void SetCutWasActive__6Camera(unsigned char* camera) {
    camera[365] = 5;
}
