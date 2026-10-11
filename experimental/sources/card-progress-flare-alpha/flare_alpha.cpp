// Observed: this flareAlpha is the 4-byte card word MC_Init stores as 128.
// Word loads and stores are the common accesses. Five floating-point loads
// use the same address. The image bytes are zero. Local symbols of the same
// name at two other addresses are different objects.
// Inferred: none.
// Unknown: what those floating-point loads expect from the integer bits.
extern "C" int flareAlpha = 0;
