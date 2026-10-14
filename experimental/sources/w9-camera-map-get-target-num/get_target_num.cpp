// Observed GetTargetNum__6Camera at 0x0026fc88, 8 bytes.
// Returns the signed byte at camera+353. Nothing is stored. Nothing here was
// executed on the EE.
extern "C" int GetTargetNum__6Camera(const unsigned char* camera) {
    return static_cast<signed char>(camera[353]);
}
